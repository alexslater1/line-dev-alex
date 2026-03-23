/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 *
 * Unified inverse Laplace transform using Abate-Whitt framework.
 * Supports CME (Concentrated Matrix Exponential), Euler, and Gaver-Stehfest methods.
 * Based on: Horvath, Horvath, Almousa, Telek - "Numerical inverse Laplace
 * transformation using concentrated matrix-exponential distributions"
 */
package jline.lib.lti

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import org.apache.commons.math3.complex.Complex
import java.util.function.UnaryOperator
import kotlin.math.floor
import kotlin.math.ln
import kotlin.math.min

object iltcme {

    // Lazy-loaded CME parameters from iltcme.json resource
    @Volatile
    private var cmeParams: List<CmeEntry>? = null

    private data class CmeEntry(
        val n: Int,
        val optim: String,
        val a: List<Double>,
        val b: List<Double>,
        val c: Double,
        val omega: Double,
        val phi: Any, // can be Double or List<Double>
        val lognorm: Double,
        val mu1: Double,
        val mu2: Double,
        val cv2: Double
    )

    private fun loadParams(): List<CmeEntry> {
        var params = cmeParams
        if (params == null) {
            synchronized(this) {
                params = cmeParams
                if (params == null) {
                    val stream = iltcme::class.java.classLoader.getResourceAsStream("iltcme.json")
                        ?: throw RuntimeException("iltcme.json not found in classpath resources")
                    val json = stream.bufferedReader().use { it.readText() }
                    val type = object : TypeToken<List<CmeEntry>>() {}.type
                    params = Gson().fromJson<List<CmeEntry>>(json, type)
                    cmeParams = params
                }
            }
        }
        return params!!
    }

    /**
     * Compute the inverse Laplace transform of fun at time points T.
     *
     * @param fun   Laplace transform function F(s), operating on Complex
     * @param T     array of time points (must be positive)
     * @param maxFnEvals  maximum number of function evaluations allowed
     * @param method  "cme" (default), "euler", or "gaver"
     * @return array of f(t) values
     */
    @JvmStatic
    @JvmOverloads
    fun ilt(
        `fun`: UnaryOperator<Complex>,
        T: DoubleArray,
        maxFnEvals: Int,
        method: String = "cme"
    ): DoubleArray {
        val eta: Array<Complex>
        val beta: Array<Complex>

        when (method) {
            "cme" -> {
                val allParams = loadParams()
                // Find the most steep CME satisfying maxFnEvals
                var best = allParams[0]
                for (i in 1 until allParams.size) {
                    val p = allParams[i]
                    if (p.cv2 < best.cv2 && p.n + 1 <= maxFnEvals) {
                        best = p
                    }
                }
                val n = best.n
                // eta = [c*mu1, (a + i*b)*mu1]
                eta = Array(n + 1) { idx ->
                    if (idx == 0) {
                        Complex(best.c * best.mu1)
                    } else {
                        Complex(best.a[idx - 1] * best.mu1, best.b[idx - 1] * best.mu1)
                    }
                }
                // beta = [1, 1 + i*(1:n)*omega] * mu1
                beta = Array(n + 1) { idx ->
                    if (idx == 0) {
                        Complex(best.mu1)
                    } else {
                        Complex(best.mu1, idx.toDouble() * best.omega * best.mu1)
                    }
                }
            }
            "euler" -> {
                val nEuler = floor((maxFnEvals - 1).toDouble() / 2.0).toInt()
                // Build eta array: [0.5, ones(n_euler), zeros(n_euler-1), 2^-n_euler]
                val etaRaw = DoubleArray(2 * nEuler + 1)
                etaRaw[0] = 0.5
                for (i in 1..nEuler) etaRaw[i] = 1.0
                // zeros(n_euler-1) already 0
                etaRaw[2 * nEuler] = Math.pow(2.0, -nEuler.toDouble())

                // Binomial accumulation using log-gamma for numerical stability
                for (k in 1 until nEuler) {
                    // eta(2*n_euler-k + 1) = eta(2*n_euler-k + 2) + exp(logsum - n_euler*log(2) - log(k!) - log((n_euler-k)!))
                    var logBinom = 0.0
                    for (i in 1..nEuler) logBinom += ln(i.toDouble())
                    logBinom -= nEuler * ln(2.0)
                    for (i in 1..k) logBinom -= ln(i.toDouble())
                    for (i in 1..(nEuler - k)) logBinom -= ln(i.toDouble())
                    etaRaw[2 * nEuler - k] = etaRaw[2 * nEuler - k + 1] + Math.exp(logBinom)
                }

                // Apply scaling: eta = 10^(n_euler/3) * (-1)^k * eta
                val scale = Math.pow(10.0, nEuler.toDouble() / 3.0)
                eta = Array(2 * nEuler + 1) { k ->
                    val sign = if (k % 2 == 0) 1.0 else -1.0
                    Complex(scale * sign * etaRaw[k])
                }

                // beta = n_euler*log(10)/3 + i*pi*k
                beta = Array(2 * nEuler + 1) { k ->
                    Complex(nEuler * ln(10.0) / 3.0, Math.PI * k.toDouble())
                }
            }
            "gaver" -> {
                var mfe = maxFnEvals
                if (mfe % 2 == 1) mfe--
                val ndiv2 = mfe / 2

                val etaArr = DoubleArray(mfe)
                val betaArr = DoubleArray(mfe)

                // Precompute log(1), log(1)+log(2), ...
                val logsum = DoubleArray(mfe + 1)
                logsum[0] = 0.0
                for (i in 1..mfe) logsum[i] = logsum[i - 1] + ln(i.toDouble())

                for (k in 1..mfe) {
                    var insideSum = 0.0
                    val jStart = floor((k + 1).toDouble() / 2.0).toInt()
                    val jEnd = min(k, ndiv2)
                    for (j in jStart..jEnd) {
                        insideSum += Math.exp(
                            (ndiv2 + 1) * ln(j.toDouble())
                                    - logsum[ndiv2 - j]
                                    + logsum[2 * j]
                                    - 2 * logsum[j]
                                    - logsum[k - j]
                                    - logsum[2 * j - k]
                        )
                    }
                    val sign = if ((k + ndiv2) % 2 == 0) 1.0 else -1.0
                    etaArr[k - 1] = ln(2.0) * sign * insideSum
                    betaArr[k - 1] = k * ln(2.0)
                }

                eta = Array(mfe) { Complex(etaArr[it]) }
                beta = Array(mfe) { Complex(betaArr[it]) }
            }
            else -> throw IllegalArgumentException(
                "Unknown inverse Laplace transform method: $method. Supported: cme, euler, gaver"
            )
        }

        // Common Abate-Whitt evaluation: f(t) = (1/t) * sum(real(eta * F(beta/t)))
        val result = DoubleArray(T.size)
        for (i in T.indices) {
            val t = T[i]
            var sum = 0.0
            for (j in eta.indices) {
                val s = beta[j].divide(t) // beta[j] / t
                val fVal = `fun`.apply(s)
                sum += eta[j].multiply(fVal).real
            }
            result[i] = sum / t
        }
        return result
    }
}
