/*
 * Copyright (c) 2012-2026, Imperial College London
 * All rights reserved.
 */
package jline.inference.api

import jline.util.matrix.Matrix

/**
 * Compute per-class queue lengths at arrival.
 *
 * Reconstructs the queue state seen by each arriving job using arrival
 * and departure times. Arrival times and response times are matched by
 * job ID, so they need not be in the same order or come from the same
 * data source.
 *
 * At ties, departures are processed before arrivals.
 *
 * @param at arrival times (n x 1)
 * @param atJobid job IDs for arrival times (n x 1)
 * @param rt response times (m x 1, m >= n)
 * @param rtJobid job IDs for response times (m x 1)
 * @param classVec class of each arrival sample (n x 1, 0-based)
 * @param R number of classes
 * @return n x R matrix of per-class queue lengths at each arrival,
 *         rows ordered consistently with the input at/atJobid
 */
fun infer_compute_ql_at_arrival(
    at: DoubleArray,
    atJobid: IntArray,
    rt: DoubleArray,
    rtJobid: IntArray,
    classVec: IntArray,
    R: Int
): Matrix {
    val n = at.size

    // Match response times to arrivals by job ID
    val rtMap = HashMap<Int, Double>(rtJobid.size)
    for (i in rtJobid.indices) {
        rtMap[rtJobid[i]] = rt[i]
    }
    val rtMatched = DoubleArray(n) { i ->
        rtMap[atJobid[i]] ?: throw IllegalArgumentException(
            "infer_compute_ql_at_arrival: not all arrival job IDs found in response time job IDs."
        )
    }

    // Sort arrivals by time
    val sortIdx = (0 until n).sortedBy { at[it] }.toIntArray()
    val atSorted = DoubleArray(n) { at[sortIdx[it]] }
    val classSorted = IntArray(n) { classVec[sortIdx[it]] }
    val rtSorted = DoubleArray(n) { rtMatched[sortIdx[it]] }

    val exitTimes = DoubleArray(n) { atSorted[it] + rtSorted[it] }

    // Event list: [time, type (-1=dep/+1=arv), sortedIdx, class]
    data class EventEntry(val time: Double, val type: Int, val sortedIdx: Int, val classIdx: Int)

    val events = ArrayList<EventEntry>(2 * n)
    for (i in 0 until n) {
        events.add(EventEntry(atSorted[i], 1, i, classSorted[i]))
        events.add(EventEntry(exitTimes[i], -1, i, classSorted[i]))
    }

    // Sort by time; departures (-1) before arrivals (+1) at same time
    events.sortWith(compareBy<EventEntry> { it.time }.thenBy { it.type })

    val state = IntArray(R)
    val qlSorted = Matrix(n, R)

    for (ev in events) {
        val c = ev.classIdx
        if (ev.type == 1) { // arrival
            state[c]++
            for (r in 0 until R) {
                qlSorted.set(ev.sortedIdx, r, state[r].toDouble())
            }
        } else { // departure
            state[c]--
        }
    }

    // Unsort back to original input order
    val ql = Matrix(n, R)
    for (si in 0 until n) {
        val origIdx = sortIdx[si]
        for (r in 0 until R) {
            ql.set(origIdx, r, qlSorted.get(si, r))
        }
    }

    return ql
}
