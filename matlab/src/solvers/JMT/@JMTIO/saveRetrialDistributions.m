function [simDoc, section] = saveRetrialDistributions(self, simDoc, section, ind)
% [SIMDOC, SECTION] = SAVERETRIALDISTRIBUTIONS(SIMDOC, SECTION, IND)
%
% Generates XML for retrial delay distributions for JMT Queue sections.
% The retrial distribution tells JMT how long a blocked customer waits
% in the orbit before retrying.
%
% Copyright (c) 2012-2026, Imperial College London
% All rights reserved.

retrialNode = simDoc.createElement('parameter');
retrialNode.setAttribute('array', 'true');
retrialNode.setAttribute('classPath', 'jmt.engine.NetStrategies.ServiceStrategy');
retrialNode.setAttribute('name', 'retrialDistributions');

sn = self.getStruct;
numOfClasses = sn.nclasses;
exportClasses = self.getExportableClasses();

% Get the actual Queue node to access retrialDelays
nodes = self.model.getNodes();
currentNode = nodes{ind};

for r=1:numOfClasses
    if ~exportClasses(r)
        continue;
    end

    refClassNode = simDoc.createElement('refClass');
    refClassNode.appendChild(simDoc.createTextNode(sn.classnames{r}));
    retrialNode.appendChild(refClassNode);

    serviceTimeStrategyNode = simDoc.createElement('subParameter');
    serviceTimeStrategyNode.setAttribute('classPath', 'jmt.engine.NetStrategies.ServiceStrategies.ServiceTimeStrategy');
    serviceTimeStrategyNode.setAttribute('name', 'ServiceTimeStrategy');

    % Check if retrial delay is defined for this class
    hasRetrial = false;
    retrialDist = [];
    if isa(currentNode, 'Queue') && ~isempty(currentNode.retrialDelays)
        if r <= size(currentNode.retrialDelays, 2) && ~isempty(currentNode.retrialDelays{1, r})
            hasRetrial = true;
            retrialDist = currentNode.retrialDelays{1, r};
        end
    end

    if ~hasRetrial
        % Default: Exp(1) placeholder
        distributionNode = simDoc.createElement('subParameter');
        distributionNode.setAttribute('classPath', 'jmt.engine.random.Exponential');
        distributionNode.setAttribute('name', 'Exponential');
        serviceTimeStrategyNode.appendChild(distributionNode);

        distrParNode = simDoc.createElement('subParameter');
        distrParNode.setAttribute('classPath', 'jmt.engine.random.ExponentialPar');
        distrParNode.setAttribute('name', 'distrPar');
        subParNodeLambda = simDoc.createElement('subParameter');
        subParNodeLambda.setAttribute('classPath', 'java.lang.Double');
        subParNodeLambda.setAttribute('name', 'lambda');
        subParValue = simDoc.createElement('value');
        subParValue.appendChild(simDoc.createTextNode('1.000000000000'));
        subParNodeLambda.appendChild(subParValue);
        distrParNode.appendChild(subParNodeLambda);
        serviceTimeStrategyNode.appendChild(distrParNode);
    else
        % Write the actual retrial distribution
        % Determine distribution type from the distribution object
        if isa(retrialDist, 'Exp')
            javaClass = 'jmt.engine.random.Exponential';
            javaParClass = 'jmt.engine.random.ExponentialPar';
            distName = 'Exponential';
        elseif isa(retrialDist, 'Erlang')
            javaClass = 'jmt.engine.random.Erlang';
            javaParClass = 'jmt.engine.random.ErlangPar';
            distName = 'Erlang';
        elseif isa(retrialDist, 'HyperExp')
            javaClass = 'jmt.engine.random.HyperExp';
            javaParClass = 'jmt.engine.random.HyperExpPar';
            distName = 'Hyperexponential';
        elseif isa(retrialDist, 'Det')
            javaClass = 'jmt.engine.random.DeterministicDistr';
            javaParClass = 'jmt.engine.random.DeterministicDistrPar';
            distName = 'Deterministic';
        elseif isa(retrialDist, 'Gamma')
            javaClass = 'jmt.engine.random.GammaDistr';
            javaParClass = 'jmt.engine.random.GammaDistrPar';
            distName = 'Gamma';
        elseif isa(retrialDist, 'Uniform')
            javaClass = 'jmt.engine.random.Uniform';
            javaParClass = 'jmt.engine.random.UniformPar';
            distName = 'Uniform';
        else
            % Fallback: treat as exponential with the distribution's rate
            javaClass = 'jmt.engine.random.Exponential';
            javaParClass = 'jmt.engine.random.ExponentialPar';
            distName = 'Exponential';
        end

        distributionNode = simDoc.createElement('subParameter');
        distributionNode.setAttribute('classPath', javaClass);
        distributionNode.setAttribute('name', distName);
        serviceTimeStrategyNode.appendChild(distributionNode);

        distrParNode = simDoc.createElement('subParameter');
        distrParNode.setAttribute('classPath', javaParClass);
        distrParNode.setAttribute('name', 'distrPar');

        if isa(retrialDist, 'Exp')
            subParNodeLambda = simDoc.createElement('subParameter');
            subParNodeLambda.setAttribute('classPath', 'java.lang.Double');
            subParNodeLambda.setAttribute('name', 'lambda');
            subParValue = simDoc.createElement('value');
            subParValue.appendChild(simDoc.createTextNode(sprintf('%.12f', retrialDist.getRate())));
            subParNodeLambda.appendChild(subParValue);
            distrParNode.appendChild(subParNodeLambda);

        elseif isa(retrialDist, 'Det')
            subParNodeT = simDoc.createElement('subParameter');
            subParNodeT.setAttribute('classPath', 'java.lang.Double');
            subParNodeT.setAttribute('name', 't');
            subParValue = simDoc.createElement('value');
            subParValue.appendChild(simDoc.createTextNode(sprintf('%.12f', retrialDist.getMean())));
            subParNodeT.appendChild(subParValue);
            distrParNode.appendChild(subParNodeT);

        elseif isa(retrialDist, 'Erlang')
            subParNodeAlpha = simDoc.createElement('subParameter');
            subParNodeAlpha.setAttribute('classPath', 'java.lang.Double');
            subParNodeAlpha.setAttribute('name', 'alpha');
            subParValue = simDoc.createElement('value');
            subParValue.appendChild(simDoc.createTextNode(sprintf('%.12f', retrialDist.getRate())));
            subParNodeAlpha.appendChild(subParValue);
            distrParNode.appendChild(subParNodeAlpha);

            subParNodeR = simDoc.createElement('subParameter');
            subParNodeR.setAttribute('classPath', 'java.lang.Long');
            subParNodeR.setAttribute('name', 'r');
            subParValue = simDoc.createElement('value');
            subParValue.appendChild(simDoc.createTextNode(sprintf('%d', retrialDist.getNumberOfPhases())));
            subParNodeR.appendChild(subParValue);
            distrParNode.appendChild(subParNodeR);

        else
            % Generic fallback: use rate as exponential
            subParNodeLambda = simDoc.createElement('subParameter');
            subParNodeLambda.setAttribute('classPath', 'java.lang.Double');
            subParNodeLambda.setAttribute('name', 'lambda');
            subParValue = simDoc.createElement('value');
            subParValue.appendChild(simDoc.createTextNode(sprintf('%.12f', 1/retrialDist.getMean())));
            subParNodeLambda.appendChild(subParValue);
            distrParNode.appendChild(subParNodeLambda);
        end

        serviceTimeStrategyNode.appendChild(distrParNode);
    end

    retrialNode.appendChild(serviceTimeStrategyNode);
end

section.appendChild(retrialNode);
end
