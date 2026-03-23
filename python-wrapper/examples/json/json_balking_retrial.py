from line_solver import *
import jpype


def json_balking_retrial():
    """Open 2-class network with balking, retrial, and patience."""
    model = Network('Balking_Retrial')

    source = Source(model, 'Source')
    queue = Queue(model, 'Queue', SchedStrategy.FCFS)
    sink = Sink(model, 'Sink')

    queue.setCapacity(15)

    class1 = OpenClass(model, 'Class1')
    class2 = OpenClass(model, 'Class2')

    source.setArrival(class1, Exp(1))
    source.setArrival(class2, Exp(0.5))
    queue.setService(class1, Exp(2))
    queue.setService(class2, Exp(3))

    # Class1: balking based on queue length (via Java API)
    BalkingThreshold = jpype.JPackage('jline').lang.constant.BalkingThreshold
    jBalkingStrategy = jpype.JPackage('jline').lang.constant.BalkingStrategy
    thresholds = jpype.java.util.ArrayList()
    thresholds.add(BalkingThreshold(5, 10, 0.3))
    thresholds.add(BalkingThreshold(11, jpype.JInt(2147483647), 1.0))
    queue.obj.setBalking(class1.obj, jBalkingStrategy.QUEUE_LENGTH, thresholds)

    # Class1: patience (reneging) via Java API
    queue.obj.setPatience(class1.obj, Exp(0.1).obj)

    # Class2: retrial with max attempts via Java API
    queue.obj.setRetrial(class2.obj, Exp(0.5).obj, 3)

    model.link(Network.serialRouting(source, queue, sink))
    return model
