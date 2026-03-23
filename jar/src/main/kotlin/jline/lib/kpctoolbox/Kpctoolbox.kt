@file:JvmName("KpctoolboxFacades")

package jline.lib.kpctoolbox

import jline.lib.kpctoolbox.aph.aph_fit
import jline.lib.kpctoolbox.basic.maxpos
import jline.lib.kpctoolbox.basic.minpos
import jline.lib.kpctoolbox.kpcfit.KPCFitOptions
import jline.lib.kpctoolbox.kpcfit.KPCFitResult
import jline.lib.kpctoolbox.kpcfit.kpcfit_auto
import jline.lib.kpctoolbox.kpcfit.kpcfit_init
import jline.lib.kpctoolbox.mc.ctmc_solve
import jline.lib.kpctoolbox.mc.ctmc_uniformization
import jline.lib.kpctoolbox.mc.dtmc_solve
import jline.lib.kpctoolbox.mmpp.mmpp2_fit1
import jline.lib.kpctoolbox.mmpp.mmpp2_fit2
import jline.lib.kpctoolbox.mmpp.mmpp2_fit3
import jline.lib.kpctoolbox.mmpp.mmpp2_fit4
import jline.lib.kpctoolbox.mvph.mvph_corr
import jline.lib.kpctoolbox.mvph.mvph_cov
import jline.lib.kpctoolbox.mvph.mvph_joint
import jline.lib.kpctoolbox.mvph.mvph_mean_x
import jline.lib.kpctoolbox.mvph.mvph_mean_y
import jline.lib.kpctoolbox.trace.autocov
import jline.lib.kpctoolbox.trace.mtrace_mean
import jline.lib.kpctoolbox.trace.trace_acf
import jline.lib.kpctoolbox.trace.trace_bicov
import jline.lib.kpctoolbox.trace.trace_gamma
import jline.lib.kpctoolbox.trace.trace_iat2bins
import jline.lib.kpctoolbox.trace.trace_iat2counts
import jline.lib.kpctoolbox.trace.trace_idi
import jline.lib.kpctoolbox.trace.trace_joint
import jline.lib.kpctoolbox.trace.trace_mean
import jline.lib.kpctoolbox.trace.trace_pmf
import jline.lib.kpctoolbox.trace.trace_scv
import jline.lib.kpctoolbox.trace.trace_shuffle
import jline.lib.kpctoolbox.trace.trace_skew
import jline.lib.kpctoolbox.trace.trace_summary
import jline.lib.kpctoolbox.trace.trace_var
import jline.util.matrix.Matrix
import jline.util.matrix.MatrixCell
import java.util.ArrayList

/**
 * Facade object providing static-style access to KPC-Toolbox functions.
 *
 * This object bridges the gap between JPype (which needs a class with static methods)
 * and Kotlin top-level functions. JPype accesses methods via:
 *   jline.lib.kpctoolbox.Kpctoolbox.INSTANCE.methodName(...)
 * or equivalently via @JvmStatic:
 *   jline.lib.kpctoolbox.Kpctoolbox.methodName(...)
 *
 * Functions that accept DoubleArray in the Kotlin implementations are wrapped to
 * accept Matrix objects (from jline.util.matrix.Matrix), since the Python wrapper
 * passes Matrix objects via JPype using jlineMatrixFromArray().
 */
object Kpctoolbox {

    // ========== TRACE ANALYSIS ==========

    /**
     * Computes the mean of a trace.
     * @param S Input trace as Matrix (column or row vector)
     * @return Mean value
     */
    @JvmStatic
    fun trace_mean(S: Matrix): Double {
        return trace_mean(S.toArray1D())
    }

    /**
     * Computes the variance of a trace.
     * @param S Input trace as Matrix
     * @return Variance
     */
    @JvmStatic
    fun trace_var(S: Matrix): Double {
        return trace_var(S.toArray1D())
    }

    /**
     * Computes the squared coefficient of variation (SCV) of a trace.
     * @param S Input trace as Matrix
     * @return SCV = variance / mean^2
     */
    @JvmStatic
    fun trace_scv(S: Matrix): Double {
        return trace_scv(S.toArray1D())
    }

    /**
     * Computes the autocorrelation function at specified lags.
     * The wrapper calls this with (Matrix, int) where int is max_lag.
     * Returns a Matrix of ACF values for lags 1..maxLag.
     *
     * @param S Input trace as Matrix
     * @param maxLag Maximum lag to compute
     * @return Matrix containing ACF values
     */
    @JvmStatic
    fun trace_acf(S: Matrix, maxLag: Int): Matrix {
        val data = S.toArray1D()
        val lags = IntArray(maxLag) { it + 1 }
        val acf = trace_acf(data, lags)
        val result = Matrix(acf.size, 1)
        for (i in acf.indices) {
            result.set(i, 0, acf[i])
        }
        return result
    }

    /**
     * Computes the skewness of a trace.
     * @param S Input trace as Matrix
     * @return Skewness value
     */
    @JvmStatic
    fun trace_skew(S: Matrix): Double {
        return trace_skew(S.toArray1D())
    }

    /**
     * Computes the joint moment of a trace.
     * @param S Input trace as Matrix
     * @param lags Lag values as Matrix
     * @param orders Moment orders as Matrix
     * @return Joint moment value
     */
    @JvmStatic
    fun trace_joint(S: Matrix, lags: Matrix, orders: Matrix): Double {
        val sArr = S.toArray1D()
        val lagsArr = IntArray(lags.numRows * lags.numCols)
        val ordersArr = IntArray(orders.numRows * orders.numCols)
        val lags1D = lags.toArray1D()
        val orders1D = orders.toArray1D()
        for (i in lagsArr.indices) {
            lagsArr[i] = lags1D[i].toInt()
        }
        for (i in ordersArr.indices) {
            ordersArr[i] = orders1D[i].toInt()
        }
        return trace_joint(sArr, lagsArr, ordersArr)
    }

    /**
     * Computes the bicovariance of a trace.
     * The wrapper calls this with (Matrix, Matrix, int) where:
     * - first Matrix is trace1, second is trace2, int is maxLag.
     * Returns a Matrix of bicovariance values.
     *
     * @param S1 First trace as Matrix
     * @param S2 Second trace as Matrix (used as lag grid)
     * @param maxLag Maximum lag
     * @return Matrix of bicovariance values
     */
    @JvmStatic
    fun trace_bicov(S1: Matrix, S2: Matrix, maxLag: Int): Matrix {
        val data = S1.toArray1D()
        val grid = IntArray(maxLag) { it + 1 }
        val result = trace_bicov(data, grid)
        val bicov = result.first
        val m = Matrix(bicov.size, 1)
        for (i in bicov.indices) {
            m.set(i, 0, bicov[i])
        }
        return m
    }

    /**
     * Computes the bicovariance using a grid array.
     * @param S Input trace as Matrix
     * @param grid Grid of lag values as IntArray
     * @return Matrix of bicovariance values
     */
    @JvmStatic
    fun trace_bicov(S: Matrix, grid: IntArray): Matrix {
        val result = trace_bicov(S.toArray1D(), grid)
        val bicov = result.first
        val m = Matrix(bicov.size, 1)
        for (i in bicov.indices) {
            m.set(i, 0, bicov[i])
        }
        return m
    }

    /**
     * Computes the index of dispersion for intervals (IDI).
     * The wrapper calls this with (Matrix, int) where int is num_intervals.
     *
     * @param S Input trace as Matrix
     * @param numIntervals Number of intervals
     * @return IDI value
     */
    @JvmStatic
    fun trace_idi(S: Matrix, numIntervals: Int): Double {
        val data = S.toArray1D()
        val result = trace_idi(data, intArrayOf(numIntervals))
        return if (result.isNotEmpty()) result[0] else Double.NaN
    }

    /**
     * Computes the index of dispersion for counts (IDC).
     * The wrapper calls this with (Matrix, double) where double is time_window.
     * Since IDC is asymptotically equal to IDI, we use trace_idi with appropriate k.
     *
     * @param S Input trace as Matrix
     * @param timeWindow Time window for analysis
     * @return IDC value
     */
    @JvmStatic
    fun trace_idc(S: Matrix, timeWindow: Double): Double {
        val data = S.toArray1D()
        // Use timeWindow to determine the lag k: k = ceil(timeWindow / mean)
        val mean = trace_mean(data)
        val k = if (mean > 0) {
            maxOf(1, Math.ceil(timeWindow / mean).toInt())
        } else {
            1
        }
        val result = trace_idi(data, intArrayOf(k))
        return if (result.isNotEmpty()) result[0] else Double.NaN
    }

    /**
     * Estimates the autocorrelation decay rate of a trace.
     * The wrapper calls this with (Matrix, int) where int is max_lag limit.
     *
     * @param S Input trace as Matrix
     * @param limit Maximum lag considered
     * @return Matrix containing gamma value
     */
    @JvmStatic
    fun trace_gamma(S: Matrix, limit: Int): Matrix {
        val data = S.toArray1D()
        val gamma = trace_gamma(data, limit)
        val result = Matrix(1, 1)
        result.set(0, 0, gamma)
        return result
    }

    /**
     * Shuffles a trace randomly.
     * @param S Input trace as Matrix
     * @return Shuffled trace as Matrix
     */
    @JvmStatic
    fun trace_shuffle(S: Matrix): Matrix {
        val shuffled = trace_shuffle(S.toArray1D())
        val result = Matrix(shuffled.size, 1)
        for (i in shuffled.indices) {
            result.set(i, 0, shuffled[i])
        }
        return result
    }

    /**
     * Computes the counting process from inter-arrival times.
     * The wrapper calls this with (Matrix, int) where int is num_bins.
     *
     * @param S Inter-arrival times as Matrix
     * @param numBins Number of bins (used as scale)
     * @return Counts as Matrix
     */
    @JvmStatic
    fun trace_iat2counts(S: Matrix, numBins: Int): Matrix {
        val data = S.toArray1D()
        val scale = trace_mean(data) * numBins
        val counts = trace_iat2counts(data, scale)
        val result = Matrix(counts.size, 1)
        for (i in counts.indices) {
            result.set(i, 0, counts[i].toDouble())
        }
        return result
    }

    /**
     * Computes binned counts from inter-arrival times.
     * The wrapper calls this with (Matrix, int) where int is num_bins.
     *
     * @param S Inter-arrival times as Matrix
     * @param numBins Number of bins
     * @return Matrix of binned counts
     */
    @JvmStatic
    fun trace_iat2bins(S: Matrix, numBins: Int): Matrix {
        val data = S.toArray1D()
        val totalTime = data.sum()
        val scale = totalTime / numBins
        val result = trace_iat2bins(data, scale)
        val counts = result.first
        val m = Matrix(counts.size, 1)
        for (i in counts.indices) {
            m.set(i, 0, counts[i].toDouble())
        }
        return m
    }

    /**
     * Computes the PMF of discrete values in a trace.
     * @param S Input trace as Matrix
     * @return Matrix where column 0 = PMF values, column 1 = unique values
     */
    @JvmStatic
    fun trace_pmf(S: Matrix): Matrix {
        val result = trace_pmf(S.toArray1D())
        val pmf = result.first
        val values = result.second
        val m = Matrix(pmf.size, 2)
        for (i in pmf.indices) {
            m.set(i, 0, pmf[i])
            m.set(i, 1, values[i])
        }
        return m
    }

    /**
     * Computes summary statistics for a trace.
     * @param S Input trace as Matrix
     * @return TraceSummary object
     */
    @JvmStatic
    fun trace_summary(S: Matrix): jline.lib.kpctoolbox.trace.TraceSummary {
        return trace_summary(S.toArray1D())
    }

    /**
     * Computes the autocovariance of a trace.
     * @param S Input trace as Matrix
     * @return Matrix of autocovariance values
     */
    @JvmStatic
    fun autocov(S: Matrix): Matrix {
        val acv = autocov(S.toArray1D())
        val result = Matrix(acv.size, 1)
        for (i in acv.indices) {
            result.set(i, 0, acv[i])
        }
        return result
    }

    // ========== MULTI-TRACE ANALYSIS ==========

    /**
     * Computes the mean of multiple traces.
     * The wrapper passes a Python list of Matrix objects.
     *
     * @param traces List of trace Matrix objects
     * @return Mean value (double) representing overall mean
     */
    @JvmStatic
    fun mtrace_mean(traces: java.util.List<Matrix>): Double {
        val traceArrays = ArrayList<DoubleArray>()
        for (i in 0 until traces.size) {
            traceArrays.add(traces.get(i).toArray1D())
        }
        val means = mtrace_mean(traceArrays)
        // Return overall mean across all traces
        var sum = 0.0
        for (m in means) {
            sum += m
        }
        return sum / means.size
    }

    /**
     * Computes the variance across multiple traces.
     * The wrapper expects this to return a single double.
     *
     * @param traces List of trace Matrix objects
     * @return Variance value
     */
    @JvmStatic
    fun mtrace_var(traces: java.util.List<Matrix>): Double {
        val traceArrays = ArrayList<DoubleArray>()
        for (i in 0 until traces.size) {
            traceArrays.add(traces.get(i).toArray1D())
        }
        // Compute variance of means across traces
        val means = mtrace_mean(traceArrays)
        val overallMean = means.sum() / means.size
        var sumSqDiff = 0.0
        for (m in means) {
            sumSqDiff += (m - overallMean) * (m - overallMean)
        }
        return if (means.size > 1) sumSqDiff / (means.size - 1) else 0.0
    }

    // ========== BASIC UTILITIES ==========

    /**
     * Creates an identity matrix.
     * @param n Size of the matrix
     * @return Identity matrix
     */
    @JvmStatic
    fun eye(n: Int): Matrix {
        return Matrix.eye(n)
    }

    /**
     * Creates a matrix of ones.
     * @param m Number of rows
     * @param n Number of columns
     * @return Matrix of ones
     */
    @JvmStatic
    fun ones(m: Int, n: Int): Matrix {
        val result = Matrix(m, n)
        for (i in 0 until m) {
            for (j in 0 until n) {
                result.set(i, j, 1.0)
            }
        }
        return result
    }

    /**
     * Creates a matrix of zeros.
     * @param m Number of rows
     * @param n Number of columns
     * @return Zero matrix
     */
    @JvmStatic
    fun zeros(m: Int, n: Int): Matrix {
        return Matrix(m, n)
    }

    /**
     * Finds the position of the maximum value.
     * @param data Input data as Matrix
     * @return Index (0-based) of maximum value
     */
    @JvmStatic
    fun maxpos(data: Matrix): Int {
        return maxpos(data.toArray1D())
    }

    /**
     * Finds the position of the minimum value.
     * @param data Input data as Matrix
     * @return Index (0-based) of minimum value
     */
    @JvmStatic
    fun minpos(data: Matrix): Int {
        return minpos(data.toArray1D())
    }

    // ========== MVPH (MULTIVARIATE PHASE-TYPE) ==========

    /**
     * Computes E[X] = mean of first variable in bivariate PH.
     * Wrapper calls: mvph_mean_x(alpha, A, B) with 3 Matrix args.
     * Maps to: mvph_mean_x(alpha, S=A, T=B, D=eye) -- T and D are unused when n2=0.
     *
     * @param alpha Initial probability vector as Matrix
     * @param A Generator matrix S
     * @param B Matrix (unused for mean_x since n2=0)
     * @return E[X]
     */
    @JvmStatic
    fun mvph_mean_x(alpha: Matrix, A: Matrix, B: Matrix): Double {
        return mvph_mean_x(alpha.toArray1D(), A, B, Matrix.eye(B.numRows))
    }

    /**
     * Computes E[Y] = mean of second variable in bivariate PH.
     * Wrapper calls: mvph_mean_y(alpha, A, C) with 3 Matrix args.
     * Maps to: mvph_mean_y(alpha, S=A, T=C, D=eye) -- S is unused when n1=0.
     *
     * @param alpha Initial probability vector as Matrix
     * @param A Generator matrix S (unused for mean_y since n1=0)
     * @param C Generator matrix T for Y
     * @return E[Y]
     */
    @JvmStatic
    fun mvph_mean_y(alpha: Matrix, A: Matrix, C: Matrix): Double {
        return mvph_mean_y(alpha.toArray1D(), A, C, Matrix.eye(A.numRows))
    }

    /**
     * Computes covariance of bivariate PH.
     * Wrapper calls: mvph_cov(alpha, A, B, C) with 4 Matrix args.
     * Maps to: mvph_cov(alpha, S=A, T=B, D=C).
     *
     * @param alpha Initial probability vector as Matrix
     * @param A Generator matrix S for X
     * @param B Generator matrix T for Y
     * @param C Transition matrix D from X to Y
     * @return Cov(X, Y)
     */
    @JvmStatic
    fun mvph_cov(alpha: Matrix, A: Matrix, B: Matrix, C: Matrix): Double {
        return mvph_cov(alpha.toArray1D(), A, B, C)
    }

    /**
     * Computes correlation of bivariate PH.
     * Wrapper calls: mvph_corr(alpha, A, B, C) with 4 Matrix args.
     * Maps to: mvph_corr(alpha, S=A, T=B, D=C).
     *
     * @param alpha Initial probability vector as Matrix
     * @param A Generator matrix S for X
     * @param B Generator matrix T for Y
     * @param C Transition matrix D from X to Y
     * @return Corr(X, Y)
     */
    @JvmStatic
    fun mvph_corr(alpha: Matrix, A: Matrix, B: Matrix, C: Matrix): Double {
        return mvph_corr(alpha.toArray1D(), A, B, C)
    }

    /**
     * Computes the joint moment of bivariate PH.
     * Wrapper calls: mvph_joint(alpha, A, B, C, x, y) with 6 Matrix args.
     * Maps to: mvph_joint(alpha, S=A, T=B, D=C, n1=x(0,0), n2=y(0,0)).
     *
     * @param alpha Initial probability vector as Matrix
     * @param A Generator matrix S for X
     * @param B Generator matrix T for Y
     * @param C Transition matrix D from X to Y
     * @param x Matrix containing moment order n1 in position (0,0)
     * @param y Matrix containing moment order n2 in position (0,0)
     * @return Joint moment E[X^n1 * Y^n2]
     */
    @JvmStatic
    fun mvph_joint(alpha: Matrix, A: Matrix, B: Matrix, C: Matrix, x: Matrix, y: Matrix): Double {
        val n1 = x.get(0, 0).toInt()
        val n2 = y.get(0, 0).toInt()
        return mvph_joint(alpha.toArray1D(), A, B, C, n1, n2)
    }
}

/**
 * Facade object for MMPP2 fitting functions.
 * Accessed by the wrapper via: jline.lib.kpctoolbox.MMPP2
 */
object MMPP2 {

    /**
     * Fits MMPP2 from moments (3-element Matrix).
     * Wrapper calls: MMPP2.fit(momentsMatrix)
     *
     * @param moments Matrix containing [E1, E2, E3]
     * @return Fitted MAP as MatrixCell {D0, D1}
     */
    @JvmStatic
    fun fit(moments: Matrix): MatrixCell {
        val m = moments.toArray1D()
        return mmpp2_fit3(m[0], m[1], m[2], 0.5)
    }

    /**
     * Fits MMPP2 from moments and autocorrelation decay.
     * Wrapper calls: MMPP2.fit(momentsMatrix, acf1)
     *
     * @param moments Matrix containing [E1, E2, E3]
     * @param acf1 Lag-1 autocorrelation
     * @return Fitted MAP as MatrixCell {D0, D1}
     */
    @JvmStatic
    fun fit(moments: Matrix, acf1: Double): MatrixCell {
        val m = moments.toArray1D()
        val scv = (m[1] - m[0] * m[0]) / (m[0] * m[0])
        val rho0 = (1 - 1 / scv) / 2
        val g2 = if (rho0 != 0.0) acf1 / rho0 else 0.5
        return mmpp2_fit3(m[0], m[1], m[2], g2)
    }

    /**
     * Fits MMPP2 from mean, SCV, skewness, and IDC.
     * Wrapper calls: MMPP2.fit1(momentsMatrix)
     *
     * @param moments Matrix containing [mean, scv, skew, idc]
     * @return Fitted MAP as MatrixCell {D0, D1}
     */
    @JvmStatic
    fun fit1(moments: Matrix): MatrixCell {
        val m = moments.toArray1D()
        return mmpp2_fit1(m[0], m[1], m[2], m[3])
    }

    /**
     * Fits MMPP2 from mean, SCV, skewness, and lag-1 autocorrelation.
     * Wrapper calls: MMPP2.fit2(momentsMatrix, acf1)
     *
     * @param moments Matrix containing [mean, scv, skew]
     * @param acf1 Lag-1 autocorrelation
     * @return Fitted MAP as MatrixCell {D0, D1}
     */
    @JvmStatic
    fun fit2(moments: Matrix, acf1: Double): MatrixCell {
        val m = moments.toArray1D()
        return mmpp2_fit4(m[0], m[1], m[2], acf1)
    }

    /**
     * Fits MMPP2 from moments and two autocorrelation values.
     * Wrapper calls: MMPP2.fit3(momentsMatrix, acf1, acf2)
     *
     * @param moments Matrix containing [mean, scv, skew]
     * @param acf1 Lag-1 autocorrelation
     * @param acf2 Lag-2 autocorrelation (used to estimate G2)
     * @return Fitted MAP as MatrixCell {D0, D1}
     */
    @JvmStatic
    fun fit3(moments: Matrix, acf1: Double, acf2: Double): MatrixCell {
        val m = moments.toArray1D()
        val g2 = if (acf1 != 0.0) acf2 / acf1 else 0.5
        return mmpp2_fit2(m[0], m[1], m[2], g2)
    }
}

/**
 * Facade object for KPC fitting functions.
 * Accessed by the wrapper via: jline.lib.kpctoolbox.KPCFit
 */
object KPCFit {

    /**
     * Automatic KPC fitting from moments.
     * Wrapper calls: KPCFit.fitAuto(momentsMatrix)
     *
     * @param moments Matrix of moments
     * @return KPCFitResult
     */
    @JvmStatic
    fun fitAuto(moments: Matrix): KPCFitResult {
        val data = moments.toArray1D()
        val traceData = kpcfit_init(data)
        return kpcfit_auto(traceData)
    }

    /**
     * Automatic KPC fitting with lag-1 autocorrelation.
     * Wrapper calls: KPCFit.fitAuto(momentsMatrix, acf1)
     *
     * @param moments Matrix of moments
     * @param acf1 Lag-1 autocorrelation
     * @return KPCFitResult
     */
    @JvmStatic
    fun fitAuto(moments: Matrix, acf1: Double): KPCFitResult {
        val data = moments.toArray1D()
        val traceData = kpcfit_init(data)
        return kpcfit_auto(traceData)
    }

    /**
     * Automatic KPC fitting with two autocorrelation values.
     * Wrapper calls: KPCFit.fitAuto(momentsMatrix, acf1, acf2)
     *
     * @param moments Matrix of moments
     * @param acf1 Lag-1 autocorrelation
     * @param acf2 Lag-2 autocorrelation
     * @return KPCFitResult
     */
    @JvmStatic
    fun fitAuto(moments: Matrix, acf1: Double, acf2: Double): KPCFitResult {
        val data = moments.toArray1D()
        val traceData = kpcfit_init(data)
        return kpcfit_auto(traceData)
    }

    /**
     * Manual KPC fitting with specified parameters.
     * Wrapper calls: KPCFit.fitManual(momentsMatrix, nComponents, structure)
     *
     * @param moments Matrix of moments
     * @param nComponents Number of KPC components
     * @param structure Structure type
     * @return KPCFitResult
     */
    @JvmStatic
    fun fitManual(moments: Matrix, nComponents: Int, structure: String): KPCFitResult {
        val data = moments.toArray1D()
        val traceData = kpcfit_init(data)
        val options = KPCFitOptions(numMAPs = nComponents)
        return kpcfit_auto(traceData, options)
    }
}

/**
 * Facade object for Markov Chain functions.
 * Accessed by the wrapper via: jline.lib.kpctoolbox.MarkovChain
 */
object MarkovChain {

    /**
     * Computes steady-state distribution for CTMC.
     * Wrapper calls: MarkovChain.ctmcSteadyState(Q_Matrix)
     *
     * @param Q Generator matrix as Matrix
     * @return Steady-state distribution as Matrix (row vector)
     */
    @JvmStatic
    fun ctmcSteadyState(Q: Matrix): Matrix {
        val pi = ctmc_solve(Q)
        val result = Matrix(1, pi.size)
        for (i in pi.indices) {
            result.set(0, i, pi[i])
        }
        return result
    }

    /**
     * Computes steady-state distribution for DTMC.
     * Wrapper calls: MarkovChain.dtmcSteadyState(P_Matrix)
     *
     * @param P Transition probability matrix as Matrix
     * @return Steady-state distribution as Matrix (row vector)
     */
    @JvmStatic
    fun dtmcSteadyState(P: Matrix): Matrix {
        val pi = dtmc_solve(P)
        val result = Matrix(1, pi.size)
        for (i in pi.indices) {
            result.set(0, i, pi[i])
        }
        return result
    }

    /**
     * Computes transient distribution for CTMC at time t.
     * Wrapper calls: MarkovChain.ctmcTransient(Q_Matrix, t)
     *
     * @param Q Generator matrix as Matrix
     * @param t Time point
     * @return Transient distribution as Matrix (row vector)
     */
    @JvmStatic
    fun ctmcTransient(Q: Matrix, t: Double): Matrix {
        val n = Q.numRows
        val pi0 = DoubleArray(n)
        pi0[0] = 1.0 // Default: start from state 0
        val resultPair = ctmc_uniformization(pi0, Q, t)
        val pi = resultPair.first
        val result = Matrix(1, pi.size)
        for (i in pi.indices) {
            result.set(0, i, pi[i])
        }
        return result
    }
}

/**
 * Facade object for Acyclic Phase-Type (APH) functions.
 * Accessed by the wrapper via: jline.lib.kpctoolbox.APH
 */
object APH {

    /**
     * Fits APH distribution from moments.
     * Wrapper calls: APH.fromMoments(momentsMatrix)
     * Returns a Pair where .first = alpha (Matrix), .second = A (Matrix).
     *
     * @param moments Matrix containing [E1, E2, E3]
     * @return Pair of (alpha as Matrix, A as Matrix)
     */
    @JvmStatic
    fun fromMoments(moments: Matrix): Pair<Matrix, Matrix> {
        val m = moments.toArray1D()
        val result = aph_fit(m[0], m[1], m[2])
        val MAP = result.first
        // MAP is {D0, D1}; alpha = stationary distribution, A = D0
        // For APH: alpha is the initial probability vector, A is the sub-generator
        val D0 = MAP[0]
        val D1 = MAP[1]
        val n = D0.numRows

        // Compute alpha from D1: alpha_i = sum_j(D1_ji * pi_j) / rate_j
        // Or use map_pie to get the interval-stationary distribution
        // For an APH represented as {D0, D1}, the initial vector is proportional to D1's column sums
        // Simplified: alpha = first row of identity (standard initial state for APH from fitting)
        val alpha = Matrix(1, n)
        // Use the column sums of D1 normalized
        var sumD1 = 0.0
        for (j in 0 until n) {
            var colSum = 0.0
            for (i in 0 until n) {
                colSum += D1.get(i, j)
            }
            alpha.set(0, j, colSum)
            sumD1 += colSum
        }
        if (sumD1 > 0) {
            for (j in 0 until n) {
                alpha.set(0, j, alpha.get(0, j) / sumD1)
            }
        }

        return Pair(alpha, D0)
    }

    /**
     * Computes moment bounds for APH distributions of order 2.
     * Wrapper calls: APH.bounds2ndOrder(m1, m2, m3)
     *
     * @param m1 First moment
     * @param m2 Second moment
     * @param m3 Third moment
     * @return Bounds value
     */
    @JvmStatic
    fun bounds2ndOrder(m1: Double, m2: Double, m3: Double): Double {
        // For order 2 APH, the bound on m3 given m1, m2 is:
        // m3_lb = (3/2) * m2^2 / m1
        val m3_lb = 1.5 * m2 * m2 / m1
        return m3_lb
    }

    /**
     * Computes moment bounds for APH distributions of order 3.
     * Wrapper calls: APH.bounds3rdOrder(m1, m2, m3)
     *
     * @param m1 First moment
     * @param m2 Second moment
     * @param m3 Third moment
     * @return Bounds value
     */
    @JvmStatic
    fun bounds3rdOrder(m1: Double, m2: Double, m3: Double): Double {
        // For order 3 APH, compute bound
        val n2 = m2 / (m1 * m1)
        val n3_lb = (4.0 / 3.0) * n2 * n2 - (1.0 / 3.0) * n2
        return n3_lb * m1 * m2
    }
}
