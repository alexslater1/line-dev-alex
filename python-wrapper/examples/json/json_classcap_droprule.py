from line_solver import *
import numpy as np


def json_classcap_droprule():
    """Open 2-class network with per-class capacity, drop rules, and load-dependence."""
    model = Network('ClassCap_DropRule')

    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.PS)
    sink = Sink(model, 'Sink')

    queue.setNumberOfServers(2)
    queue.setCapacity(20)

    class1 = OpenClass(model, 'Class1')
    class2 = OpenClass(model, 'Class2')

    source.setArrival(class1, Exp(1))
    source.setArrival(class2, Exp(0.5))
    queue.setService(class1, Exp(3))
    queue.setService(class2, Exp(2))

    # Per-class capacity (via Java API - not wrapped on Queue)
    queue.obj.setClassCap(class1.obj, 8)
    queue.obj.setClassCap(class2.obj, 15)

    # Per-class drop rules (via Java API - not wrapped on Queue)
    queue.obj.setDropRule(class1.obj, DropStrategy.Drop.value)
    queue.obj.setDropRule(class2.obj, DropStrategy.WaitingQueue.value)

    # Load-dependent scaling
    queue.setLoadDependence(np.array([1.0, 0.9, 0.8, 0.7]))

    model.link(Network.serialRouting(source, queue, sink))
    return model
