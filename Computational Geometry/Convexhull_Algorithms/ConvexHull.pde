Edge findMiddleLine(ArrayList<Point> points)
{
  Point leftHighest = points.get(0);
  Point rightLowest = points.get(0);
  int indexL = 0;
  int indexR = 0;
  for(int i = 0; i < points.size(); i++){
    Point currentPoint = points.get(i);
    if (currentPoint.getX() <= leftHighest.getX()) {
      if (currentPoint.getX() == leftHighest.getX()){
        if(currentPoint.getY() > leftHighest.getY()){
          leftHighest = currentPoint;
          indexL = i;
        }
      }
      else{
          leftHighest = currentPoint;
          indexL = i;
      }
    }
    if (currentPoint.getX() >= rightLowest.getX()) {
      if (currentPoint.getX() == rightLowest.getX()){
        if(currentPoint.getY() < rightLowest.getY()){
          rightLowest = currentPoint;
          indexR = i;
        }
      }
      else{
          rightLowest = currentPoint;
          indexR = i;
      }
    }
  }
  return new Edge(leftHighest, rightLowest);
}

boolean inLeftHalf(Edge middleLine, Point currentPoint){
  if(currentPoint.getX() != middleLine.p0.getX() && currentPoint.getY() != middleLine.p0.getY() && currentPoint.getX() != middleLine.p1.getX() && currentPoint.getY() != middleLine.p1.getY()){
    float ax = middleLine.p1.getX() - middleLine.p0.getX();
    float ay = middleLine.p1.getY() - middleLine.p0.getY();
    float bx = currentPoint.getX() - middleLine.p0.getX();
    float by = currentPoint.getY() - middleLine.p0.getY();
    //float crossProd = (ax*by) - (ay*bx);
    float crossProd = (ax * by) - ( ay * bx);
    if(crossProd > 0){
      return true;
    }
    else {
      return false;
    }
  }
  return false;
}

Point findExtremePoint(Edge middleLine, ArrayList<Point> points){
  if(points.size() != 0){
    Point extremePoint = points.get(0);
    float distance = 0.0;
    for(int i = 0; i < points.size(); i++){
      Point currentPoint = points.get(i);
      if(currentPoint.getX() != middleLine.p0.getX() && currentPoint.getY() != middleLine.p0.getY() && currentPoint.getX() != middleLine.p1.getX() && currentPoint.getY() != middleLine.p1.getY()){
        float vx = middleLine.p1.getX() - middleLine.p0.getX();
        float vy = middleLine.p1.getY() - middleLine.p0.getY();
        float wx = currentPoint.getX() - middleLine.p0.getX();
        float wy = currentPoint.getY() - middleLine.p0.getY();

        float lengthSquared = vx * vx + vy * vy;
        float dotProduct = vx * wx + vy * wy;
        float t = dotProduct / lengthSquared;
        float closestX = middleLine.p0.getX() + t * vx;
        float closestY = middleLine.p0.getY() + t * vy;
        Point closestPoint = new Point(closestX, closestY);
        if( distance < currentPoint.distance(closestPoint)){
          extremePoint = currentPoint;
          distance = currentPoint.distance(closestPoint);
        }
      }
    }
    if(extremePoint.getX() == middleLine.p0.getX() && extremePoint.getY() == middleLine.p0.getY() || extremePoint.getX() == middleLine.p1.getX() && extremePoint.getY() == middleLine.p1.getY()){
      return null;
    }
    return extremePoint;
  }
  return null;
}

Polygon recurrsiveMethodLeft(Polygon cHull, Edge middleLine, ArrayList<Point> points){
  ArrayList<Point> upperPoints = new ArrayList<>();
  for(int i = 0; i < points.size(); i++){
    boolean isLeft = inLeftHalf(middleLine, points.get(i));
    if(isLeft){
      upperPoints.add(points.get(i));
    }
  }
  if(upperPoints.size() != 0){
    Point upperExtreme = findExtremePoint(middleLine, upperPoints);
    if(upperExtreme != null){
      Edge upperLeftMiddleLine = new Edge(middleLine.p0, upperExtreme);
      Edge upperRightMiddleLine = new Edge(middleLine.p1, upperExtreme);
      cHull = recurrsiveMethodRight(cHull, upperRightMiddleLine, upperPoints);
      cHull.addPoint(upperExtreme);
      cHull = recurrsiveMethodLeft(cHull, upperLeftMiddleLine, upperPoints);
    }
  }
  return cHull;
}
Polygon recurrsiveMethodRight(Polygon cHull, Edge middleLine, ArrayList<Point> points){
  ArrayList<Point> upperPoints = new ArrayList<>();
  for(int i = 0; i < points.size(); i++){
    boolean isLeft = inLeftHalf(middleLine, points.get(i));
    if(!isLeft){
      upperPoints.add(points.get(i));
    }
  }
  if(upperPoints.size() != 0){
    Point upperExtreme = findExtremePoint(middleLine, upperPoints);
    if(upperExtreme != null){
      Edge upperLeftMiddleLine = new Edge(middleLine.p1, upperExtreme);
      Edge upperRightMiddleLine = new Edge(middleLine.p0, upperExtreme);
      cHull = recurrsiveMethodRight(cHull, upperRightMiddleLine, upperPoints);
      cHull.addPoint(upperExtreme);
      cHull = recurrsiveMethodLeft(cHull, upperLeftMiddleLine, upperPoints);
    }
  }
  return cHull;
}

Polygon ConvexHullQuickHull( ArrayList<Point> points ){
  Polygon cHull = new Polygon();
  if(points.size() >= 3){
    Edge middleLine = findMiddleLine(points);
    cHull.addPoint(middleLine.p1);
    ArrayList<Point> upperPoints = new ArrayList<>();
    ArrayList<Point> lowerPoints = new ArrayList<>();

    for(int i = 0; i < points.size(); i++){
      boolean isLeft = inLeftHalf(middleLine, points.get(i));
      if(isLeft){
        upperPoints.add(points.get(i));
      }
      else{
        lowerPoints.add(points.get(i));
      }
    }
    Point upperExtreme = findExtremePoint(middleLine, upperPoints);
    Point lowerExtreme = findExtremePoint(middleLine, lowerPoints);
    if(upperExtreme != null){
      Edge upperLeftMiddleLine = new Edge(middleLine.p0, upperExtreme);
      Edge upperRightMiddleLine = new Edge(middleLine.p1, upperExtreme);
      cHull = recurrsiveMethodRight(cHull, upperRightMiddleLine, upperPoints);
      cHull.addPoint(upperExtreme);
      cHull = recurrsiveMethodLeft(cHull, upperLeftMiddleLine, upperPoints);
    }
    cHull.addPoint(middleLine.p0);
    if(lowerExtreme != null){
      Edge lowerLeftMiddleLine = new Edge(middleLine.p0, lowerExtreme);
      Edge lowerRightMiddleLine = new Edge(middleLine.p1, lowerExtreme);
      cHull = recurrsiveMethodRight(cHull, lowerLeftMiddleLine, lowerPoints);
      cHull.addPoint(lowerExtreme);
      cHull = recurrsiveMethodLeft(cHull, lowerRightMiddleLine, lowerPoints);
    }
  }
  return cHull;
}

Point findLeftmostPoint(ArrayList<Point> points){
  Point leftmostPoint = points.get(0);
  for(int i = 0; i < points.size(); i++){
    Point currentPoint = points.get(i);
    if (currentPoint.getX() < leftmostPoint.getX()) {
        leftmostPoint = currentPoint;
    }
  }
  return leftmostPoint;
}

Stack<Point> checkforConvexity(Stack<Point> stack){
  if(stack.size() >= 3){
    Point p = stack.pop();
    Point q = stack.pop();
    Point r = stack.pop();

    float ax = q.getX() - r.getX();
    float ay = q.getY() - r.getY();
    float bx = p.getX() - r.getX();
    float by = p.getY() - r.getY();
    //float crossProd = (ax*by) - (ay*bx);
    float crossProd = (ax * by) - ( ay * bx);
    if(crossProd > 0){
      stack.push(r);
      stack.push(q);
      stack.push(p);
    }
    else {
      stack.push(r);
      stack.push(p);
      stack = checkforConvexity(stack);
    }
  }
  return stack;
}

Polygon ConvexHullGraham( ArrayList<Point> points ){
  Polygon cHull = new Polygon();
  if(points.size() >= 3){
    Point leftmostPoint  = findLeftmostPoint(points);
    Collections.sort(points, new Comparator<Point>() {
            @Override
            public int compare(Point p1, Point p2) {

                float angle = 0;
                PVector v1 = new PVector(p1.getX() - leftmostPoint.getX(), p1.getY() - leftmostPoint.getY());
                PVector v2 = new PVector(p2.getX() - leftmostPoint.getX(), p2.getY() - leftmostPoint.getY());
                PVector v3 = new PVector(0.0, -1000.0 - leftmostPoint.getY());
                double angle1 = PVector.angleBetween(v1, v3);
                double angle2 = PVector.angleBetween(v2, v3);
                if (angle1 < angle2) {
                    return -1;
                } else if (angle1 > angle2) {
                    return 1;
                } else {
                    return 0;
                }
            }
        });
    Stack<Point> pointsStack = new Stack<Point>();
    pointsStack.push(points.get(0));
    pointsStack.push(points.get(1));
    pointsStack.push(points.get(2));
    for(int i = 3; i < points.size(); i++){
      pointsStack.push(points.get(i));
      pointsStack = checkforConvexity(pointsStack);
    }
    while(!pointsStack.empty()){
      cHull.addPoint(pointsStack.pop());
    }
  }
  return cHull;
}

Point findLowestPoint(ArrayList<Point> points){
  Point lowestPoint = points.get(0);
  int index = 0;
  for(int i = 0; i < points.size(); i++){
    Point currentPoint = points.get(i);
    if (currentPoint.getY() < lowestPoint.getY()) {
        lowestPoint = currentPoint;
        index = i;
    }
  }
  return lowestPoint;
}

Point findHighestPoint(ArrayList<Point> points){
  Point highestPoint = points.get(0);
  int index = 0;
  for(int i = 0; i < points.size(); i++){
    Point currentPoint = points.get(i);
    if (currentPoint.getY() > highestPoint.getY()) {
        highestPoint = currentPoint;
        index = i;
    }
  }
  return highestPoint;
}

float findPolarAngle(Point referencePoint, Point currentPoint, Boolean negAxisFlag){
  float angle = 0;
  PVector v1 = new PVector(currentPoint.getX() - referencePoint.getX(), currentPoint.getY() - referencePoint.getY());
  PVector v2 = new PVector(1000.0 - referencePoint.getX(), 0.0);
  PVector v3 = new PVector(-1000.0 - referencePoint.getX(), 0.0);
  if(!negAxisFlag){
    angle = PVector.angleBetween(v2, v1);
  }
  else{
    angle = PVector.angleBetween(v3, v1);
  }
  return angle;
}

Point addPointWithLeastPolarAngle(Polygon cHull, Point referencePoint, ArrayList<Point> points, Boolean negAxisFlag, Point highestPoint, Point lowestPoint){
  Point cHullVertex = lowestPoint;
  float polarAngle = 3.15;
  int index =0;
  for(int i=0; i< points.size(); i++){
    Point currentPoint = points.get(i);
    Boolean considerHigher = negAxisFlag ? currentPoint.getY() < referencePoint.getY() : currentPoint.getY() > referencePoint.getY();
    if(currentPoint.getX() != referencePoint.getX() && currentPoint.getY() != referencePoint.getY() && considerHigher){
      float currentPolarAngle = findPolarAngle(referencePoint, currentPoint, negAxisFlag);
        if(currentPolarAngle <= polarAngle){
          // if(currentPolarAngle == polarAngle)
          polarAngle = currentPolarAngle;
          cHullVertex = currentPoint;
          index = i;
        }
      }
  }
  return cHullVertex;
}

Polygon ConvexHullGiftWrap( ArrayList<Point> points ){
  Polygon cHull = new Polygon();
  if(points.size() >= 3){
    Point lowestPoint  = findLowestPoint(points);
    Point highestPoint  = findHighestPoint(points);
    Point referencePoint = lowestPoint;
    Boolean negAxisFlag = false;
    // Point cHullVertex = points.get(0);
    cHull.addPoint(lowestPoint);
    do{
      Point cHullVertex = addPointWithLeastPolarAngle(cHull, referencePoint, points, negAxisFlag, highestPoint, lowestPoint);
      if(cHullVertex.getX() == highestPoint.getX() && cHullVertex.getY() == highestPoint.getY()){
        negAxisFlag = true;
      }
      cHull.addPoint(cHullVertex);
      referencePoint = cHullVertex;
    }while(lowestPoint.getX() != referencePoint.getX() && lowestPoint.getY() != referencePoint.getY());
  }
  return cHull;
}
