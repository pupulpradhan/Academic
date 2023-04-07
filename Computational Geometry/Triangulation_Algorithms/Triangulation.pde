Point convertToCircularLinkedList(ArrayList<Point> points) {
  Point head = new Point(points.get(0).p.x, points.get(0).p.y);
  Point current = head;
  for(int i = 1; i < points.size(); i++){
    Point newPoint = new Point(points.get(i).p.x, points.get(i).p.y);  
    current.next = newPoint;
    current = newPoint;
  }
  current.next = head; 
  return head;
}

boolean isValid2(Edge edge, Point head){
   boolean flag = false;
   ArrayList<Edge> boundary = new ArrayList<Edge>();
   Point currentPoint = head.next;
   while(currentPoint != head){
   Edge polyEdge =  new Edge(currentPoint, currentPoint.next);
   boundary.add(polyEdge);
   currentPoint = currentPoint.next;
   }
   Edge polyEdgeLast =  new Edge(currentPoint, currentPoint.next); //add last edge
   boundary.add(polyEdgeLast);
   for(int j  = 0 ; j< boundary.size(); j++){
      if(boundary.get(j).intersectionTest(edge)){
        flag = false;
        break;
      }
      flag = true;
    }
    if(flag){
      float midX = (edge.p0.p.x + edge.p1.p.x) / 2;
      float midY = (edge.p0.p.y + edge.p1.p.y) / 2;
      if(pointInNewPolygon(new Point(midX, midY), boundary)){
        flag = true;
      }
      else
      flag = false;
    }
  return flag;
 }

 public void printList(Point head) {
    Point current = head;
    do {
        System.out.print("Node: "+ current.p + " ");
        current = current.next;
    } while (current != head);
    System.out.println();
}

ArrayList<Triangle> triangulate(ArrayList<Triangle> ret, Point head){
  Point temp = head;
  // System.out.println("head : " + head);
  // System.out.println("head.next : " + head.next);
  // System.out.println("head.next.next : " + head.next.next);
  int result = 1;
  if (head != null)
  {
    while(temp.next != head){
      result++;
      temp = temp.next;
    }
  }
  if(result < 3){
    
    return ret;
  }
  if(result == 3){
    Triangle tri = new Triangle(head, head.next, head.next.next);
    ret.add(tri);
    return ret;
  }
  //get a valid diagonal from head
  //Point diagonalPoint1 = head;
  Point end = head.next.next;
  
  while(!isValid2(new Edge(head, end), head)){
      end = end.next;
      if(end  == head){
        head = head.next;
        end = head.next.next;
      }
  } //end = 5


  Point poly1head = new Point(head.p.x, head.p.y);
  Point poly1headcurrent = poly1head;
  Point current = head.next;
  while(current != end){
    Point newPoint = new Point(current.p.x, current.p.y);  
    poly1headcurrent.next = newPoint;
    poly1headcurrent = poly1headcurrent.next;
    current = current.next;
  }
  Point newPoint1 = new Point(current.p.x, current.p.y); 
  newPoint1.next = poly1head;
  poly1headcurrent.next = newPoint1;
  ArrayList<Triangle> ret1 = triangulate(ret, poly1head);
  //printList(poly1head);

  Point poly2head = new Point(current.p.x, current.p.y);
  Point poly2headcurrent = poly2head;
  current = current.next;
  while(current != head){
    Point newPoint = new Point(current.p.x, current.p.y);  
    poly2headcurrent.next = newPoint;
    poly2headcurrent = poly2headcurrent.next;
    current = current.next;
  }
  Point newPoint2 = new Point(current.p.x, current.p.y); 
  newPoint2.next = poly2head;
  poly2headcurrent.next = newPoint2;
  ArrayList<Triangle> ret2 = triangulate(ret, poly2head);
  //printList(poly2head);


  //printList(endNext);
  // Point head2 = end; 


  // Point currentPoint = head; //1st circular linked list start
  // while(currentPoint != end){
  //   currentPoint = currentPoint.next;
  // }
  // System.out.println("checkpoint4 ");
  // end.next = null;
  // Point head2 = currentPoint;//2nd circular linked list start
  // currentPoint.next = head; // 1st circular linked list complete
  // Point currentPoint2 = head2;
  // while(currentPoint2 != head){
  //   currentPoint2 = currentPoint2.next;
  // }
  // System.out.println("checkpoint5 ");
  // currentPoint2.next = head2;//2nd circular linked list complete
  // printList(head2);
  
  //ArrayList<Triangle> ret2 = triangulate(ret , head2);
  ret.addAll(ret1);
  ret.addAll(ret2);
  return ret;
 }
 
 ArrayList<Triangle> getDiagonalBasedTriangulation(Polygon poly){
   
   // TODO: Triangulate the polygon using a method of your choice
   ArrayList<Triangle> ret = new ArrayList<Triangle>();
   ArrayList<Point> vertices = new ArrayList<Point>(poly.p);
   Point head = null;
   if(vertices.size()>3){
    head = convertToCircularLinkedList(vertices);
    ArrayList<Triangle> retAll = triangulate(ret, head);
    ret.addAll(retAll);
    
   }
   else if(vertices.size() == 3){
    Triangle tri = new Triangle(vertices.get(0), vertices.get(1), vertices.get(2));
    ret.add(tri);
    return ret;
   }
   return ret;
 }

 boolean pointInNewPolygon( Point _p, ArrayList<Edge> boundary ){
     // TODO: Check if the point p is inside of the 
     //ArrayList<Edge> bdry = getBoundary();
     Edge p_ray = new Edge(_p, new Point(190.0, 170.0));
     int counter = 0;
     for(int  i = 0; i < boundary.size(); i++){
       if(boundary.get(i).intersectionTest(p_ray)){
         counter++;
       }
     }
     if(counter%2 == 0) //even # of intersections means outside the polygon
       return false;
     else
       return true;
   }
 
 boolean isValid(Edge edge, ArrayList<Point> vertices){
   boolean flag = false;
   ArrayList<Edge> boundary = new ArrayList<Edge>();
   for(int j  = 0 ; j< vertices.size(); j++){
      Edge polyEdge =  new Edge(vertices.get(j), vertices.get((j+1)%vertices.size()));
      boundary.add(polyEdge);
   }
   for(int j  = 0 ; j< boundary.size(); j++){
      if(boundary.get(j).intersectionTest(edge)){
        flag = false;
        break;
      }
      flag = true;
    }
    if(flag){
      float midX = (edge.p0.p.x + edge.p1.p.x) / 2;
      float midY = (edge.p0.p.y + edge.p1.p.y) / 2;
      if(pointInNewPolygon(new Point(midX, midY), boundary)){
        flag = true;
      }
      else
      flag = false;
    }
  return flag;
 }

 ArrayList<Triangle> getEarBasedTriangulation(Polygon ply){
 // TODO: Triangulate the polygon using a method of your choice
   ArrayList<Triangle> ret = new ArrayList<Triangle>();
   ArrayList<Point> vertices = new ArrayList<Point>(ply.p);
   ArrayList<Point> eartips = new ArrayList<Point>();
   ArrayList<Integer> indexArray = new ArrayList<>();
   if(vertices.size()>3){
   for(int i=0 ; i<vertices.size(); i++){
     if(isValid(new Edge(vertices.get((i + vertices.size() - 1) % vertices.size()), vertices.get((i + 1) % vertices.size())), vertices)){
         eartips.add(vertices.get(i));
         indexArray.add(i);
      }
    }
    int i = 0;
    while(vertices.size()>2 && eartips.size()>i){
      Point eartip = eartips.get(i);
      for(int j = 0; j< vertices.size(); j++){
         if(eartip == vertices.get(j)){
           Triangle tri = new Triangle(vertices.get((j + vertices.size() - 1) % vertices.size()), vertices.get(j), vertices.get((j +1 ) % vertices.size()));  
           ret.add(tri);
           vertices.remove(j);  
           eartips.remove(i);
           i--;
           if(j >= vertices.size()){
             break;
           }
           if(isValid(new Edge(vertices.get((j + vertices.size() - 1) % vertices.size()), vertices.get((j +1 ) % vertices.size())), vertices)){  
             int index = eartips.indexOf(vertices.get(j));
             if(index == -1){  
                eartips.add(vertices.get(j));  
              }  
            }
            else{
              int index = eartips.indexOf(vertices.get(j));
               if(index != -1){  
                  eartips.remove(vertices.get(j));  
              } 
            }
            
            if(isValid(new Edge(vertices.get((j + vertices.size()-2) % vertices.size()), vertices.get(j)), vertices)){  
             int index = eartips.indexOf(vertices.get((j + vertices.size() - 1) % vertices.size()));
             if(index == -1){  
                eartips.add(vertices.get((j + vertices.size() - 1) % vertices.size()));  
              }  
            }
            else{
              int index = eartips.indexOf(vertices.get((j + vertices.size() - 1) % vertices.size()));
               if(index != -1){  
                  eartips.remove(vertices.get((j + vertices.size() - 1) % vertices.size()));  
              } 
            }
         }
         continue;
      }
      i++;
    }
   }
  return ret;
 }
