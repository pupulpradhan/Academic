


void makeRandomPointsVersion1(){
  points.clear();
  for( int i = 0; i < numOfPoints; i++){
    points.add( new Point( random(150,200), random(150,200) ) );
    points.add( new Point( random(400,550), random(500,700) ) );
    points.add( new Point( random(600,750), random(150,200) ) );
  }
}

void makeRandomPointsVersion2(){
  points.clear();
  // TODO
  for( int i = 0; i < numOfPoints; i++){
    points.add( new Point( random(300,width-200), random(300,height-200) ) );
    points.add( new Point( 150, 150 ) );
    points.add( new Point( 550, 700 ) );
    points.add( new Point( 750, 200) ) ;
  }
}

void makeRandomPointsVersion3(){
  points.clear();
  // TODO
  for( int i = 0; i < numOfPoints; i++){
    points.add( new Point( random(500,700), random(500,700) ) );
  }
}
