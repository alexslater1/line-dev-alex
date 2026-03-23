/*
 * Copyright (c) 2012-2026, QORE Lab, Imperial College London
 * All rights reserved.
 * Ported from BUTools V2.0
 */
package jline.lib.butools.ph

import jline.util.matrix.Matrix

/**
 * Result class for PH representations containing alpha (initial vector) and A (generator matrix).
 */
data class PHRepresentation(val alpha: Matrix, val A: Matrix)
