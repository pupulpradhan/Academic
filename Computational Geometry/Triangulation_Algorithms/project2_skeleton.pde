
import java.util.*;

ArrayList<Point>    points     = new ArrayList<Point>();
Polygon             poly       = new Polygon();


boolean saveImage = false;
boolean showPotentialDiagonals = true;
boolean showDiagonals = true;
boolean showPointInside = true;


void setup(){
  size(800,800,P3D);
  frameRate(30);
}


void draw(){
  background(255);
  
  translate( 0, height, 0);
  scale( 1, -1, 1 );
  
  strokeWeight(3);
  
  fill(0);
  noStroke();
  for( Point p : points ){
    p.draw();
  }
  
  if( showPointInside ){
    fill(200);
    for(int x = 40; x < width; x+=40){
      for(int y =40; y< height; y +=40){
        if( poly.p.size() > 2 && !poly.pointInPolygon(new Point(x,y) ) ) fill(255,100,100);
        if( poly.p.size() > 2 && poly.pointInPolygon(new Point(x,y) ) ) fill(100,255,100);
        ellipse(x,y,4,4);
      }
    }
  }

  noStroke();
  // for( Triangle t : getDiagonalBasedTriangulation( poly ) ){
  //    fill( 100, 100, 100 );
  //    if( t.ccw() ) fill( 100, 100, 100, 50 );
  //    if( t.cw()  ) fill( 100, 100, 100, 50 ); 
  //    t.draw();
  //  }
   

  for( Triangle t : getEarBasedTriangulation( poly ) ){
    fill( 0, 0, 100 );
    if( t.ccw() ) fill( 0, 0, 100, 50 );
    if( t.cw()  ) fill( 0, 0, 100, 50 ); 
    t.draw();
  }
  
  stroke( 100, 100, 100 );
  if( poly.ccw() ) stroke( 100, 200, 100 );
  if( poly.cw()  ) stroke( 200, 100, 100 ); 
  poly.draw();
  
  
  if( showPotentialDiagonals ){
    strokeWeight(1);
    stroke(100,100,100);
    fill(0,0,200);
    for( Edge e : poly.getPotentialDiagonals() ){
        e.drawDotted();
    }
  }
  
  if( showDiagonals ){
    strokeWeight(6);
    stroke(100,100,200);
    for( Edge e : poly.getDiagonals() ){
        e.draw();
    }
    
    // strokeWeight(4);
    // stroke(200,100,000);
    // for( Edge e : poly.getDiagonalsOptimized() ){
    //     e.draw();
    // }    
  }
  
  fill(0);
  stroke(0);
  textSize(18);
  
  textRHC( "Controls", 10, height-20 );
  textRHC( "d: Show/Hide Diagonals", 10, height-40 );
  textRHC( "p: Show/Hide Potential Diagonals", 10, height-60 );
  textRHC( "i: Show/Hide Point Inside Check", 10, height-80 );
  textRHC( "c: Clear Polygon", 10, height-100 );
  textRHC( "s: Save Image", 10, height-120 );
  

  textRHC( "Clockwise: " + (poly.cw()?"True":"False"), 550, 80 );
  textRHC( "Counterclockwise: " + (poly.ccw()?"True":"False"), 550, 60 );
  textRHC( "Closed Boundary: " + (poly.isClosed()?"True":"False"), 550, 40 );
  textRHC( "Simple Boundary: " + (poly.isSimple()?"True":"False"), 550, 20 );
  textRHC( "Area: " + poly.area() + " px", 550, 100 );
  
  for( int i = 0; i < points.size(); i++ ){
    textRHC( i+1, points.get(i).p.x+5, points.get(i).p.y+15 );
  }
  
  if( saveImage ) saveFrame( ); 
  saveImage = false;
  
}


void keyPressed(){
  if( key == 's' ) saveImage = true;
  if( key == 'c' ){ points.clear(); poly = new Polygon(); }
  if( key == 'p' ) showPotentialDiagonals = !showPotentialDiagonals;
  if( key == 'd' ) showDiagonals = !showDiagonals;
  if( key == 'i' ) showPointInside = !showPointInside;
}


void textRHC( int s, float x, float y ){
  textRHC( Integer.toString(s), x, y );
}


void textRHC( String s, float x, float y ){
  pushMatrix();
  translate(x,y);
  scale(1,-1,1);
  text( s, 0, 0 );
  popMatrix();
}

Point sel = null;

void mousePressed(){
  int mouseXRHC = mouseX;
  int mouseYRHC = height-mouseY;
  
  float dT = 6;
  for( Point p : points ){
    float d = dist( p.p.x, p.p.y, mouseXRHC, mouseYRHC );
    if( d < dT ){
      dT = d;
      sel = p;
    }
  }
  
  if( sel == null ){
    sel = new Point(mouseXRHC,mouseYRHC);
    points.add( sel );
    poly.addPoint( sel );
  }
}

void mouseDragged(){
  int mouseXRHC = mouseX;
  int mouseYRHC = height-mouseY;
  if( sel != null ){
    sel.p.x = mouseXRHC;   
    sel.p.y = mouseYRHC;   
  }
}

void mouseReleased(){
  sel = null;
}




  
