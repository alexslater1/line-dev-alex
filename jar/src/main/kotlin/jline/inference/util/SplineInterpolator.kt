/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.util

import org.apache.commons.math3.analysis.interpolation.SplineInterpolator as CMSplineInterpolator

/**
 * Spline interpolation utility using Apache Commons Math3.
 */
object SplineInterpolator {

    /**
     * Interpolate data at new x-values using cubic spline interpolation.
     *
     * @param xOld original x-values (sorted, distinct)
     * @param yOld original y-values
     * @param xNew new x-values at which to evaluate
     * @return interpolated y-values at xNew
     */
    @JvmStatic
    fun interpolate(xOld: DoubleArray, yOld: DoubleArray, xNew: DoubleArray): DoubleArray {
        if (xOld.size < 2) {
            // Not enough points for interpolation, return constant
            return DoubleArray(xNew.size) { if (yOld.isNotEmpty()) yOld[0] else 0.0 }
        }

        val interpolator = CMSplineInterpolator()
        val function = interpolator.interpolate(xOld, yOld)

        return DoubleArray(xNew.size) { i ->
            val x = xNew[i]
            // Clamp to domain
            val xClamped = Math.max(xOld.first(), Math.min(xOld.last(), x))
            function.value(xClamped)
        }
    }
}
