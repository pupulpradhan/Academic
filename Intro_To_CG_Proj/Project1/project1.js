// bgImg is the background image to be modified.
// fgImg is the foreground image.
// fgOpac is the opacity of the foreground image.
// fgPos is the position of the foreground image in pixels. It can be negative and (0,0) means the top-left pixels of the foreground and background are aligned.
function composite( bgImg, fgImg, fgOpac, fgPos )
{
  function setColor(img, x, y, c){ //sets the RGBA for a pixel, given c = [r,g,b,a]
    let data = img.data;
    let idx = coordToIdx(x,y,img.width);
    data[idx] = c[0];
    data[idx+1] = c[1];
    data[idx+2] = c[2];
    data[idx+3] = c[3];
  }

  function coordToIdx(x,y,w){//adjust x and y to get index of pixel using width of img w
    return((y*w+x)*4);
  }

  function getColor(img, x, y){ //gets the color of a pixel
    let data = img.data;
    let idx = coordToIdx(x,y,img.width);
    return([data[idx],data[idx+1],data[idx+2],data[idx+3]]);
  }

  function getColorAlphaBlend(fgC, bgC){ //retrives the color of a pixel using alpha blending
    let fgA = fgC[3] / 255; //standardize alpha between 0 and 1
    fgA = fgA * fgOpac;
    let bgA = bgC[3] / 255; //standardize alpha between 0 and 1
    let tA = ((fgA + (1-fgA)*bgA)); //total standardized alpha
    let r = calculateNewColor(fgC,fgA,bgC,bgA,0,tA);
    let g = calculateNewColor(fgC,fgA,bgC,bgA,1,tA);
    let b = calculateNewColor(fgC,fgA,bgC,bgA,2,tA);
    let a = 255 * tA; //0-255 alpha
    return([r,g,b,a]);
  }

  function calculateNewColor(fgC,fgA,bgC,bgA,n,tA){
    if(tA==0){ // if alpha is zero, prevent divide by zero
      return(0);
    }
    return (((fgC[n] * fgA) + (bgC[n]*(1-fgA)*bgA)))/tA;
  }

  let canvas = document.createElement('canvas');
  canvas.width = bgImg.width;
  canvas.height = bgImg.height;
  let context = canvas.getContext('2d');

  for(let x = 0; x < fgImg.width; x++){ //for every pixel in the fg img, update color of bg img
    for(let y = 0; y < fgImg.height; y++){
      let modX = x+fgPos.x, modY = y+fgPos.y; //modified x and y for origin of fg image in relation to bg
      if(modX>=0 && modX<bgImg.width){ //ensure pixel location is within background borders
        setColor(bgImg, modX, modY, getColorAlphaBlend(getColor(fgImg, x, y), getColor(bgImg, modX, modY)));
      }
    }
  }
  context.putImageData(bgImg,0,0); //update image data
  fgImg.src = canvas.toDataURL();
}
