

// Module for testing performance of implementations
// works by testing may random sets of points
void performanceTest(){
  int maxNumOfPoints = 10000;
  int iterations = 1000; 
  println("Running Performance Test");
  
  println( "\n  Testing with version 1 random points:");
  for( int nP = 10; nP <= maxNumOfPoints; nP*=10, iterations/=8 ){
    numOfPoints = nP;
    println( nP + " points per test, " + iterations + " iterations" );
    
    // QuickHull
    int start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion1();
      ConvexHullQuickHull( points );
    }
    int tNaive = (millis()-start);
    println( "QuickHull Implementation: " + tNaive + " ms" );
    
    // Graham
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion1();
      ConvexHullGraham( points );
    }
    int tShamos  = (millis()-start);
    print( "Graham Implementation: " + tShamos + " ms " );
    println( "Improvement over QuickHull: " + (int)( 100.0f *(tNaive-tShamos)/(tNaive) ) + "%" );
    
    // Gift-Wrapping
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion1();
      ConvexHullGiftWrap( points );
    }
    int tAABB  = (millis()-start);
    
    print( "Gift-Wrapping Implementation: " + tAABB + " ms " );
    println( "Improvement over QuickHull: " + (int)( 100.0f *(tNaive-tAABB)/(tNaive) ) + "%" );
    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------  
  }
  
  iterations = 100;
  println( "\n  Testing with version 2 random points:");
  for( int nP = 10; nP <= maxNumOfPoints; nP*=10, iterations/=8 ){
    numOfPoints = nP;
    println( nP + " points per test; " + iterations + " iterations" );
    
    // QuickHull
    int start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion2();
      ConvexHullQuickHull( points );
    }
    int tNaive = (millis()-start);
    println( "Quickhull Implementation: " + tNaive + " ms" );
    
    // Gift-Wrapping
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion2();
      ConvexHullGiftWrap( points );
    }
    int tAABB  = (millis()-start);
    print( "Gift-Wrapping Implementation: " + tAABB + " ms " );
    println( "Improvement over Quickhull: " + (int)( 100.0f *(tNaive-tAABB)/(tNaive) ) + "%" );
    
    // Graham
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion2();
      ConvexHullGraham( points );
    }
    int tShamos  = (millis()-start);
    print( "Graham Implementation: " + tShamos + " ms " );
    println( "Improvement over Quickhull: " + (int)( 100.0f *(tNaive-tShamos)/(tNaive) ) + "%" );
    
    //----------------------------------------------------------------------------------------------------------------------------------------------------------
  }
  
  iterations = 100;
  println( "\n  Testing with version 3 random segments:");
  for( int nP = 10; nP <= maxNumOfPoints; nP*=10, iterations/=8 ){
    numOfPoints = nP;
    println(nP + " points per test; " + iterations + " iterations" );
    // QuickHull
    int start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion3();
      ConvexHullQuickHull( points );
    }
    int tNaive = (millis()-start);
    println( "QuickHull Implementation: " + tNaive + " ms" );
    
    
    // Gift-Wrapping
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion3();
      ConvexHullGiftWrap( points );
    }
    int tAABB  = (millis()-start);
    print( "Gift-Wrapping Implementation: " + tAABB + " ms " );
    println( "Improvement over QuickHull: " + (int)( 100.0f *(tNaive-tAABB)/(tNaive) ) + "%" );
    
    
    // Graham
    start = millis();
    for(int i = 0; i < iterations; i++ ){
      makeRandomPointsVersion3();
      ConvexHullGraham( points );
    }
    int tShamos  = (millis()-start);
    print( "Graham Implementation: " + tShamos + " ms " );
    println( "Improvement over QuickHull: " + (int)( 100.0f *(tNaive-tShamos)/(tNaive) ) + "%" );
    
  }
  
}
