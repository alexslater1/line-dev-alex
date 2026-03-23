from line_solver import *


def json_signal_classes():
    """Open network with G-network negative signal."""
    model = Network('SignalClasses')

    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    sink = Sink(model, 'Sink')

    class1 = OpenClass(model, 'Class1')
    signal1 = Signal(model, 'Signal1', SignalType.NEGATIVE)
    signal1.obj.forJobClass(class1.obj)
    signal1.removal_policy = RemovalPolicy.RANDOM

    source.setArrival(class1, Exp(2))
    source.setArrival(signal1, Exp(0.5))
    queue.setService(class1, Exp(5))
    queue.setService(signal1, Immediate())

    P = model.initRoutingMatrix()
    P.set(class1, class1, source, queue, 1.0)
    P.set(class1, class1, queue, sink, 1.0)
    P.set(signal1, signal1, source, queue, 1.0)
    P.set(signal1, signal1, queue, sink, 1.0)
    model.link(P)
    return model
