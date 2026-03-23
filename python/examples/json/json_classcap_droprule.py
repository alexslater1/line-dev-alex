from line_solver import *
import numpy as np


def json_classcap_droprule():
    """Open 2-class network with per-class capacity, drop rules, and load-dependence."""
    model = Network('ClassCap_DropRule')

    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.PS)
    sink = Sink(model, 'Sink')

    queue.set_number_of_servers(2)
    queue.set_capacity(20)

    class1 = OpenClass(model, 'Class1')
    class2 = OpenClass(model, 'Class2')

    source.set_arrival(class1, Exp(1))
    source.set_arrival(class2, Exp(0.5))
    queue.set_service(class1, Exp(3))
    queue.set_service(class2, Exp(2))

    # Per-class capacity
    queue.set_class_capacity(class1, 8)
    queue.set_class_capacity(class2, 15)

    # Drop rule (station-level in Python native)
    queue.set_drop_rule(DropStrategy.DROP)

    # Load-dependent scaling
    queue.set_load_dependence(np.array([1.0, 0.9, 0.8, 0.7]))

    model.link(Network.serial_routing(source, queue, sink))
    return model
