

class Edge{
  
   Point p0,p1;
      
   Edge( Point _p0, Point _p1 ){
     p0 = _p0; p1 = _p1;
   }
      
   void draw(){
     line( p0.p.x, p0.p.y, 
           p1.p.x, p1.p.y );
   }
   
   void drawDotted(){
     float steps = p0.distance(p1)/6;
     for(int i=0; i<=steps; i++) {
       float x = lerp(p0.p.x, p1.p.x, i/steps);
       float y = lerp(p0.p.y, p1.p.y, i/steps);
       //noStroke();
       ellipse(x,y,3,3);
     }
  }
   
  public String toString(){
    return "<" + p0 + "" + p1 + ">";
  }
     
  Point midpoint( ){
    return new Point( PVector.lerp( p0.p, p1.p, 0.5f ) );     
  }
     
  boolean intersectionTest( Edge other ){ 
    // TODO: Insert your project 1 code here 
    // Should return true if intersecting and false otherwise.
     boolean flag = false;
     float p0p1_x = p1.getX() - p0.getX();  //a
     float p0p1_y = p1.getY() - p0.getY();  //a
     float p0otherp0_x = other.p0.getX() - p0.getX();  //b
     float p0otherp0_y = other.p0.getY() - p0.getY();  //b
     float p0otherp1_x = other.p1.getX() - p0.getX();  //c
     float p0otherp1_y = other.p1.getY() - p0.getY();  //c
     
     float other_p0p1_x = other.p1.getX() - other.p0.getX();   //l
     float other_p0p1_y = other.p1.getY() - other.p0.getY();  //l
     float otherp0p0_x = p0.getX() - other.p0.getX();  //m
     float otherp0p0_y = p0.getY() - other.p0.getY();  //m
     float otherp0p1_x = p1.getX() - other.p0.getX();  //n
     float otherp0p1_y = p1.getY() - other.p0.getY();  //n
     
     //float crossProd = (ax*by) - (ay*bx);
     float crossProd1 = (p0p1_x * p0otherp0_y) - (p0p1_y * p0otherp0_x);
     float crossProd2 = (p0p1_x * p0otherp1_y) - (p0p1_y * p0otherp1_x);
     float crossProd3 = (other_p0p1_x * otherp0p0_y) - (other_p0p1_y * otherp0p0_x);
     float crossProd4 = (other_p0p1_x * otherp0p1_y) - (other_p0p1_y * otherp0p1_x);
     if(Math.signum(crossProd1) == -Math.signum(crossProd2) && Math.signum(crossProd3) == -Math.signum(crossProd4))
       flag = true;
     return flag; 
  }
  
}
