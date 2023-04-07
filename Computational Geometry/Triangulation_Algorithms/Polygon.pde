

class Polygon {
  
   ArrayList<Point> p     = new ArrayList<Point>();
   ArrayList<Edge>  bdry = new ArrayList<Edge>();
     
   Polygon( ){  }
   
   
   boolean isClosed(){ return p.size()>=3; }
   
   
   boolean isSimple(){
     // TODO: Check the boundary to see if it is simple or not.
     ArrayList<Edge> bdry = getBoundary();
     boolean flag = false;
     for(int i = 0; i < bdry.size() - 1; i++){
       for(int j = i+1 ; j < bdry.size(); j++){
         if(bdry.get(i).intersectionTest(bdry.get(j))){
           return false;
         }
         flag = true;
       }
     }
     return flag;
   }
   
   
   boolean pointInPolygon( Point _p ){
     // TODO: Check if the point p is inside of the 
     ArrayList<Edge> bdry = getBoundary();
     Edge p_ray = new Edge(_p, new Point(190.0, 170.0));
     int counter = 0;
     for(int  i = 0; i < bdry.size(); i++){
       if(bdry.get(i).intersectionTest(p_ray)){
         counter++;
       }
     }
     if(counter%2 == 0) //even # of intersections means outside the polygon
       return false;
     else
       return true;
   }
   
   
   ArrayList<Edge> getDiagonals(){
     // TODO: Determine which of the potential diagonals are actually diagonals
     ArrayList<Edge> bdry = getBoundary();
     ArrayList<Edge> diag = getPotentialDiagonals();
     ArrayList<Edge> ret  = new ArrayList<Edge>();
     for(int i  = 0 ; i< diag.size(); i++){
       boolean flag = false;
       for(int j  = 0 ; j< bdry.size(); j++){
         if(bdry.get(j).intersectionTest(diag.get(i))){
           flag = false;
           break;
         }
         flag = true;
       }
       if(flag){
         float midX = (diag.get(i).p0.p.x + diag.get(i).p1.p.x) / 2;
         float midY = (diag.get(i).p0.p.y + diag.get(i).p1.p.y) / 2;
         if(pointInPolygon(new Point(midX, midY))){
           ret.add(diag.get(i));
         }
       }
     }
     return ret;
   }

   boolean isDiagonalValid(Edge diagonal){
     ArrayList<Edge> bdry = getBoundary();
     boolean flag = false;
       for(int j  = 0 ; j< bdry.size(); j++){
         if(bdry.get(j).intersectionTest(diagonal)){
           flag = false;
           break;
         }
         flag = true;
       }
       if(flag){
         float midX = (diagonal.p0.p.x + diagonal.p1.p.x) / 2;
         float midY = (diagonal.p0.p.y + diagonal.p1.p.y) / 2;
         if(pointInPolygon(new Point(midX, midY))){
           flag = true;
         }
         else
          flag = false;
       }
     return flag; 
   }
   
   // from GeeksforGeeks
   double pointToEdgeDistance(Edge edge, Point point) {

        double Ax = edge.p0.p.x;
        double Ay = edge.p0.p.y;
        double Bx = edge.p1.p.x;
        double By = edge.p1.p.y;
        double Px = point.p.x;
        double Py = point.p.y;

        // Calculate the distance between the point and the line segment
        double numerator = Math.abs((Ax - Px) * (By - Ay) - (Bx - Ax) * (Ay - Py));
        double denominator = Math.sqrt(Math.pow(Bx - Ax, 2) + Math.pow(By - Ay, 2));
        double distance = numerator / denominator;

        // // Determine the slope and intercept of the line containing the line segment
        // double slope = (edge.p1.p.y - edge.p0.p.y) / (edge.p1.p.x - edge.p0.p.x);
        // double intercept = edge.p0.p.y - slope * edge.p0.p.x;
        
        // // Find the slope and intercept of the line that is perpendicular to the line segment
        // double perpendicularSlope = -1.0 / slope;
        // double perpendicularIntercept = point.p.y - perpendicularSlope * point.p.x;
        
        // // Find the intersection point of the two lines
        // double x = (perpendicularIntercept - intercept) / (slope - perpendicularSlope);
        // double y = perpendicularSlope * x + perpendicularIntercept;
        
        // // Calculate the distance between the intersection point and the given point
        // double distance = Math.sqrt(Math.pow(x - point.p.x, 2) + Math.pow(y - point.p.y, 2));
        
        return distance;
    }

   double angleBetweenVectors(Edge e0, Edge e1){
      double e1x = e1.p1.p.x - e1.p0.p.x;
      double e1y = e1.p1.p.y - e1.p0.p.y;
      double e0x = e0.p1.p.x - e0.p0.p.x;
      double e0y = e0.p1.p.y - e0.p0.p.y;
      double dotProduct = e1x * e0x + e1y * e0y;
      double mag1 = Math.sqrt(e0x * e0x + e0y * e0y);
      double mag2 = Math.sqrt(e1x * e1x + e1y * e1y);
      double cosTheta = dotProduct / (mag1 * mag2);
      double angle = Math.toDegrees(Math.acos(cosTheta));
      return angle;
   }

   ArrayList<Edge> getDiagonalsOptimized(){
     // TODO: Determine which of the potential diagonals are actually diagonals
     ArrayList<Edge> ret  = new ArrayList<Edge>();
     ArrayList<Edge> boundary = new ArrayList<Edge>(bdry);
     if(p.size() > 3){
     for(Point point : p){ //main loop of the algo for all the vertices
       ArrayList<Point> edgeListForEachPoint = new ArrayList<>(); 
       Collections.sort(boundary, new Comparator<Edge>(){
         public int compare(Edge e0, Edge e1){
           if(pointToEdgeDistance(e0, point) < pointToEdgeDistance(e1, point)) return -1;
           if(pointToEdgeDistance(e0, point) > pointToEdgeDistance(e1, point)) return 1;
           return 0;
         }
       });
       Edge e1 = null;
       Edge e2 = null;
      for(Edge polyEdge : boundary){
        if(polyEdge.p0 == point){
          e1 = polyEdge;
        }
        if(polyEdge.p1 == point){
          e2 = polyEdge;
        }
      }
      double baseAngle = angleBetweenVectors(e1,e2);
      for(int i=2 ; i<boundary.size(); i++){
        double angleTocheckp0 = angleBetweenVectors(new Edge(point, boundary.get(i).p0), boundary.get(0));
        double angleTocheckp1 = angleBetweenVectors(new Edge(point, boundary.get(i).p1), boundary.get(0));
        if(angleTocheckp0 >= baseAngle && angleTocheckp1 >= baseAngle){
          continue;
        }
        if(edgeListForEachPoint.size() == 0){
        if(angleTocheckp0 >= baseAngle && angleTocheckp1 < baseAngle){
          edgeListForEachPoint.add(boundary.get(i).p1);
        }
        if(angleTocheckp0 < baseAngle && angleTocheckp1 >= baseAngle){
          edgeListForEachPoint.add(boundary.get(i).p0);
        }
        if(angleTocheckp0 < baseAngle && angleTocheckp1 < baseAngle){
          edgeListForEachPoint.add(boundary.get(i).p0);
          edgeListForEachPoint.add(boundary.get(i).p1);
        }
        }
        else{
          //edgeListForEachPoint.get(size()-1)
          if(angleTocheckp0 >= baseAngle && angleTocheckp1 < baseAngle){
            if(angleTocheckp1 < angleBetweenVectors(new Edge(point, edgeListForEachPoint.get(edgeListForEachPoint.size()-1)), boundary.get(0))){
              edgeListForEachPoint.add(boundary.get(i).p1);
            }
          }
          if(angleTocheckp0 < baseAngle && angleTocheckp1 >= baseAngle){
            if(angleTocheckp0 < angleBetweenVectors(new Edge(point, edgeListForEachPoint.get(edgeListForEachPoint.size()-1)), boundary.get(0))){
              edgeListForEachPoint.add(boundary.get(i).p0);
            }
          }
          if(angleTocheckp0 < baseAngle && angleTocheckp1 < baseAngle){
          if(angleTocheckp0 < angleBetweenVectors(new Edge(point, edgeListForEachPoint.get(edgeListForEachPoint.size()-1)), boundary.get(0))){
              edgeListForEachPoint.add(boundary.get(i).p0);
            }
          if(angleTocheckp1 < angleBetweenVectors(new Edge(point, edgeListForEachPoint.get(edgeListForEachPoint.size()-1)), boundary.get(0))){
              edgeListForEachPoint.add(boundary.get(i).p1);
            }
          }
        }
        if(edgeListForEachPoint.size() > 0 && i==2){
        Collections.sort(edgeListForEachPoint, new Comparator<Point>(){
          public int compare(Point p0, Point p1){
           if(angleBetweenVectors(new Edge(point, p0), boundary.get(0)) < angleBetweenVectors(new Edge(point, p1), boundary.get(0))) return -1;
           if(angleBetweenVectors(new Edge(point, p0), boundary.get(0)) > angleBetweenVectors(new Edge(point, p1), boundary.get(0))) return 1;
           return 0;
          }
        });
        }
      }
      for(int i = 0; i < edgeListForEachPoint.size(); i++ ){
        boolean flag = true;
          for(Edge polyEdge : boundary){
            if(polyEdge.intersectionTest(new Edge(point, edgeListForEachPoint.get(i) ))){
              flag = false;
            }
          }
          if(flag){
            ret.add(new Edge(point, edgeListForEachPoint.get(i) ));
          }  
      }
     }
     }
     return ret;
   }
      
   
   
   boolean ccw(){
     // TODO: Determine if the polygon is oriented in a counterclockwise fashion
     if( !isClosed() ) return false;
     if( !isSimple() ) return false;
     //ArrayList<Edge> bdry = getBoundary();
     float crossProds = 0.0;
     for( int  i = 0 ; i < bdry.size() - 1; i++ ){
       //float crossProd = (ax*by) - (ay*bx);
       Edge a  = bdry.get(i);
       Edge b = bdry.get(i+1);
       float ax = a.p1.getX() - a.p0.getX();
       float ay = a.p1.getY() - a.p0.getY();
       float bx = b.p1.getX() - b.p0.getX();
       float by = b.p1.getY() - b.p0.getY();
       //float crossProd = (ax*by) - (ay*bx);
       crossProds += (bx - ax) * (by + ay);
     }
     if(crossProds > 0)
         return false;
     return true;
   }
   
   
   boolean cw(){
     // TODO: Determine if the polygon is oriented in a clockwise fashion
     if( !isClosed() ) return false;
     if( !isSimple() ) return false;
     if(ccw()){
       return false;
     }
     return true;
   }
   
   float area(){
     // TODO: Calculate and return the area of the polygon
     float x[] = new float[p.size()];
     float y[] = new float[p.size()];
     float area = 0;
     if(p.size()>3){
     for (int k = 0; k< p.size()-1;k++){
       x[k] = p.get(k).p.x;
       y[k] = p.get(k).p.y;
     }
     int j = x.length - 1;
     for (int i = 0; i < x.length; i++) {
        area += (x[j] * y[i]) - (x[i] * y[j]);
        j = i;
      }
      area /= 2;
     }
     return Math.abs(area);  
   }
      

   ArrayList<Edge> getBoundary(){
     return bdry;
   }


   ArrayList<Edge> getPotentialDiagonals(){
     ArrayList<Edge> ret = new ArrayList<Edge>();
     int N = p.size();
     for(int i = 0; i < N; i++ ){
       int M = (i==0)?(N-1):(N);
       for(int j = i+2; j < M; j++ ){
         ret.add( new Edge( p.get(i), p.get(j) ) );
       }
     }
     return ret;
   }
   

   void draw(){
     //println( bdry.size() );
     for( Edge e : bdry ){
       e.draw();
     }
   }
   
   
   void addPoint( Point _p ){ 
     p.add( _p );
     if( p.size() == 2 ){
       bdry.add( new Edge( p.get(0), p.get(1) ) );
       bdry.add( new Edge( p.get(1), p.get(0) ) );
     }
     if( p.size() > 2 ){
       bdry.set( bdry.size()-1, new Edge( p.get(p.size()-2), p.get(p.size()-1) ) );
       bdry.add( new Edge( p.get(p.size()-1), p.get(0) ) );
     }
   }

}
