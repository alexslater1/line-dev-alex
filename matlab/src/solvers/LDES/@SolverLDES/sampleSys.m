function tranSysState = sampleSys(self, numEvents)
% TRANSYSSTATE = SAMPLESYS(NUMSAMPLES) Returns system-wide sample path
%
% @brief Generates a discrete-event sample path for all stateful nodes
%
% This method runs a LDES transient simulation and records the sequence
% of states visited at all stateful nodes in the network.
%
% @param self SolverLDES instance
% @param numEvents Number of time points to sample
%
% @return tranSysState Structure containing the system-wide sample path with fields:
%         - tranSysState.handle: Cell array of stateful node handles
%         - tranSysState.t: Vector of time points
%         - tranSysState.state: Cell array of state matrices (one per node)
%         - tranSysState.event: Cell array of events
%         - tranSysState.isaggregate: Boolean false (detailed sample)
%
% @see sampleSysAggr - Returns system-wide sample path aggregated over phases
% @see sample - Returns sample path for a single node
%
% Example:
% @code
% model = Network('MM1');
% % ... model setup ...
% solver = SolverLDES(model, 'seed', 42);
% result = solver.sampleSys(1000);
% @endcode

if nargin < 2
    numEvents = self.options.samples;
end

% Convert model to Java
jmodel = LINE2JLINE(self.model);

% Create Java LDES options
joptions = jline.solvers.ldes.LDESOptions();
joptions.samples = numEvents;
joptions.seed = self.options.seed;

% Create Java solver
jsolver = jline.solvers.ldes.SolverLDES(jmodel, joptions);

% Call Java sampleSys method
jresult = jsolver.sampleSys(numEvents);

% Convert Java result to MATLAB structure
tranSysState = struct();
tranSysState.handle = self.model.getStatefulNodes';

% Convert time matrix
if ~isempty(jresult.t)
    tranSysState.t = JLINE.from_jline_matrix(jresult.t);
else
    tranSysState.t = [];
end

% Convert state - it's a List<Matrix> in Java, convert to cell array
sn = self.model.getStruct;
if ~isempty(jresult.state) && isa(jresult.state, 'java.util.List')
    nstateful = jresult.state.size();
    tranSysState.state = cell(1, nstateful);
    for isf = 1:nstateful
        jmatrix = jresult.state.get(isf - 1);  % Java is 0-indexed
        if ~isempty(jmatrix) && isa(jmatrix, 'jline.util.matrix.Matrix')
            tranSysState.state{isf} = JLINE.from_jline_matrix(jmatrix);
        else
            tranSysState.state{isf} = [];
        end
    end
else
    tranSysState.state = {};
end

% Convert event matrix
if ~isempty(jresult.event) && isa(jresult.event, 'jline.util.matrix.Matrix')
    tranSysState.event = JLINE.from_jline_matrix(jresult.event);
else
    tranSysState.event = {};
end

tranSysState.isaggregate = jresult.isAggregate;
tranSysState.numEvents = jresult.numEvents;

end
