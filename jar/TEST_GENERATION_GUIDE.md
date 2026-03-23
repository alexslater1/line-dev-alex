# Guide for Generating AvgTable Comparison Tests

This guide documents the process for creating unit tests that compare solver outputs between MATLAB and Java implementations for models that produce AvgTable results.

## Overview

The goal is to create unit tests that:
1. Run a model example in MATLAB and capture the AvgTable output
2. Run the same model in Java using multiple solvers
3. Compare the numerical results in detail
4. Document any differences between implementations

## Step-by-Step Process

### 1. Identify the Model Example

First, identify the example model you want to test. Look for examples in:
- MATLAB: `matlab/examples/` 
- Java: `jar/src/main/kotlin/jline/examples/java/`

Example: `cache_replc_rr` model found in:
- `matlab/examples/basic/cacheModel/cache_replc_rr.m`
- `jar/src/main/kotlin/jline/examples/java/basic/CacheModel.java`

### 2. Run the MATLAB Example

**CRITICAL: All expected values MUST come from running the example in the dev/ directory.**

Create a MATLAB script to run the example and capture output in a structured format:

```matlab
% Run example and capture results for unit test
cd '../dev'
lineStart
addpath(genpath('../matlab/examples'))

% Run the example
run('../matlab/examples/basic/cacheModel/cache_replc_rr.m');

% Display results in a format we can capture
fprintf('\n=== Results for Unit Test ===\n');
for i = 1:length(solver)
    fprintf('\nSolver %d: %s\n', i, class(solver{i}));
    disp(AvgTable{i});
end
```

Execute the script and save the output, paying attention to:
- The order of nodes and classes in the table
- **Exact numerical values from MATLAB output** - these will be your expected values
- Any solver-specific differences

**REMEMBER: The values from this dev/ MATLAB output are the ONLY values that should be used as expected values in tests.**

### 3. Analyze the Table Structure

Understand the output structure:
- **Rows**: Each row represents a node-class combination
- **Order**: Typically nodes are grouped together with all classes for each node
- **Metrics**: Standard columns are QLen, Util, RespT, ResidT, ArvR, Tput

Example for cache_replc_rr with 3 nodes × 3 classes = 9 rows:
```
Index 0-2: Source (InitClass, HitClass, MissClass)
Index 3-5: Cache (InitClass, HitClass, MissClass)  
Index 6-8: Sink (InitClass, HitClass, MissClass)
```

### 4. Create the Test Class

Create a test class following this template:

```java
package jline.examples;

import jline.examples.java.basic.[ModelClass];
import jline.lang.Network;
import jline.solvers.NetworkAvgNodeTable;
import jline.solvers.ctmc.SolverCTMC;
import jline.solvers.mva.SolverMVA;
import jline.solvers.nc.SolverNC;
import jline.util.Maths;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static jline.TestTools.MID_TOL;
import static jline.TestTools.relativeTolerance;

public class [ModelName]Test {

    @BeforeAll
    public static void setUp() {
        // Ensure MATLAB-compatible random number generation
        Maths.setRandomNumbersMatlab(true);
    }
    
    // Include the utility method for assertions
    private static void assertTableMetrics(NetworkAvgNodeTable avgTable,
                                         double[] expectedQLen,
                                         double[] expectedUtil,
                                         double[] expectedRespT,
                                         double[] expectedResidT,
                                         double[] expectedArvR,
                                         double[] expectedTput) {
        // ... (copy from BasicExamplesTest.java)
    }
```

### 5. Create Test Methods for Each Solver

For each solver (CTMC, MVA, NC, etc.), create a test method:

```java
@Test
public void test[ModelName][SolverName]() {
    // Create the model
    Network model = [ModelClass].[model_method]();
    
    // Create and run the solver
    Solver[NAME] solver = new Solver[NAME](model, [parameters]);
    NetworkAvgNodeTable avgTable = solver.getAvgNodeTable();
    
    // Check if results are computed
    assertNotNull(avgTable);
    
    // Expected values based on dev/ MATLAB output
    // CRITICAL: These values MUST come from running the example in dev/ directory
    // Order: [Document the order of nodes and classes]
    double[] expectedQLen = {/* exact values from dev/ MATLAB output */};
    double[] expectedUtil = {/* exact values from dev/ MATLAB output */};
    double[] expectedRespT = {/* exact values from dev/ MATLAB output */};
    double[] expectedResidT = {/* exact values from dev/ MATLAB output */};
    double[] expectedArvR = {/* exact values from dev/ MATLAB output */};
    double[] expectedTput = {/* exact values from dev/ MATLAB output */};
    
    // Verify table size
    assertEquals([expected_size], avgTable.getQLen().size(), 
        "Expected [N] entries ([nodes] nodes × [classes] classes)");
    
    // Check all metrics against expected values
    assertTableMetrics(avgTable, expectedQLen, expectedUtil, expectedRespT, 
                      expectedResidT, expectedArvR, expectedTput);
}
```

### 6. Handle Implementation Differences

**IMPORTANT: ALWAYS use the dev/ MATLAB values as expected values in tests, even when Java results differ.**

When Java results differ from MATLAB:

1. **ALWAYS use MATLAB values** in the expected arrays:
   ```java
   // Use MATLAB values from dev/ output as expected
   double[] expectedTput = {1.1808, 0.0, 0.0, 0.0, 1.1808, 0.0, 0.0, 0.0, 0.0};
   ```

2. **Tests should fail if Java differs from MATLAB** - This is the intended behavior to identify implementation differences

### 7. Common Patterns to Watch For

1. **Open models**: Often have all zeros for QLen, Util, RespT, ResidT
2. **Source nodes**: Typically show throughput in Tput column
3. **Cache nodes**: Show arrival rates in ArvR, throughput split between hit/miss in Tput
4. **Sink nodes**: Show arrival rates in ArvR column

### 8. Debugging Tips

If unsure about the table structure, create a debug program:

```java
public class Debug[ModelName] {
    public static void main(String[] args) {
        Network model = [ModelClass].[model_method]();
        Solver[NAME] solver = new Solver[NAME](model, [parameters]);
        NetworkAvgNodeTable avgTable = solver.getAvgNodeTable();
        
        System.out.println("Node names: " + avgTable.getNodeNames());
        System.out.println("Class names: " + avgTable.getClassNames());
        System.out.println("Throughputs: " + avgTable.getTput());
        
        avgTable.print();
    }
}
```

### 9. Best Practices

1. **Use exact numerical values from dev/ MATLAB** - Don't use placeholder checks like `> 0`
2. **Test multiple solvers** - Each solver may have different behavior
3. **Always use MATLAB values as expected** - Tests should fail if Java differs
4. **Keep tests focused** - Only test AvgTable outputs, not other metrics
5. **Use the utility function** - Avoid repeating assertion loops

### 10. Example Test Structure

For a model with 3 nodes and 2 classes (6 table entries):

```java
// Expected values for solver
// Order: Node1(Class1,Class2), Node2(Class1,Class2), Node3(Class1,Class2)
double[] expectedQLen = {0.5, 0.3, 1.2, 0.8, 0.0, 0.0};
double[] expectedUtil = {0.4, 0.2, 0.9, 0.7, 0.0, 0.0};
// ... continue for all 6 metrics
```

## Complete Example

See `BasicExamplesTest.java` for a complete implementation testing the `cache_replc_rr` model with three solvers (CTMC, MVA, NC).

## Checklist for New Tests

- [ ] Identified MATLAB and Java example files
- [ ] Ran MATLAB example and captured output
- [ ] Created test class with proper imports
- [ ] Added `@BeforeAll` setup method for MATLAB compatibility
- [ ] Included `assertTableMetrics` utility method
- [ ] Created test methods for each solver
- [ ] Documented the node-class order in comments
- [ ] Filled in expected values from MATLAB/Java output
- [ ] Documented any implementation differences
- [ ] Verified all tests pass
- [ ] Removed any non-AvgTable assertions