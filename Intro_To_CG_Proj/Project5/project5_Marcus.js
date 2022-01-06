// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	// form the transformation matrix.
	var trans = [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translationX, translationY, translationZ, 1
	];


     // homogeneous rotation coordinates 
    var rotX = [1, 0, 0, 0,
                0, Math.cos(rotationX), Math.sin(rotationX), 0,
                0 , -1*Math.sin(rotationX), Math.cos(rotationX), 0,
                0, 0, 0, 1];

    var rotY = [Math.cos(rotationY), 0, -1*Math.sin(rotationY), 0,
                0, 1, 0, 0,
                Math.sin(rotationY) , 0, Math.cos(rotationY), 0,
                0, 0, 0, 1];


    // apply transformations
    trans = MatrixMult( trans, rotX) ;  

    trans = MatrixMult( trans, rotY );

	return trans;
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
				// initializations
		this.prog = InitShaderProgram( meshVS, meshFS );

		this.mvp = gl.getUniformLocation( this.prog, 'mvp' );

		this.mmv = gl.getUniformLocation(this.prog, 'mmv');
		this.mn = gl.getUniformLocation(this.prog, 'mn');

		this.vertP = gl.getAttribLocation( this.prog, 'vertP' );

		this.sampler = gl.getUniformLocation( this.prog, 'tex' );

		this.txc = gl.getAttribLocation( this.prog, 'txc' );


		this.norm = gl.getAttribLocation( this.prog, 'norm'); // 3rd normals attrib 

		//TODO: getUniformLocation for light direction uniform
		this.lightdir = gl.getUniformLocation( this.prog, 'lightdir');

		//material properties
		this.shiny = gl.getUniformLocation( this.prog, 'shiny');

        this.showTex = gl.getUniformLocation( this.prog, 'showTex' );
       
        this.swapAx = gl.getUniformLocation( this.prog, 'swapAx' );

        this.loadedImg = gl.getUniformLocation( this.prog, 'loadedImg');       

        this.numTriangles = 0;


		// init buffers, one for vertex positon  and another for texture coordinates,  bind and fill later
        gl.useProgram( this.prog );

		this.vertBuffer = gl.createBuffer(); 

        this.texBuffer = gl.createBuffer(); 

        this.normBuffer = gl.createBuffer();

        this.myTex = gl.createTexture();
    
        //init uniforms for fragment shader
        gl.uniform1i( this.loadedImg, false);
        gl.uniform1i( this.swapAx, false);
        gl.uniform1i( this.showTex, true);	
	}
	
	// This method is called every time the user opens an OBJ file.
	// The arguments of this function is an array of 3D vertex positions,
	// an array of 2D texture coordinates, and an array of vertex normals.
	// Every item in these arrays is a floating point value, representing one
	// coordinate of the vertex position or texture coordinate.
	// Every three consecutive elements in the vertPos array forms one vertex
	// position and every three consecutive vertex positions form a triangle.
	// Similarly, every two consecutive elements in the texCoords array
	// form the texture coordinate of a vertex and every three consecutive 
	// elements in the normals array form a vertex normal.
	// Note that this method can be called multiple times.
	setMesh( vertPos, texCoords, normals )
	{
		// Update the contents of the vertex buffer objects.
		this.numTriangles = vertPos.length / 3;

		// vertex positions
        gl.bindBuffer( gl.ARRAY_BUFFER, this.vertBuffer );
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

    
        // texture coordinates
        gl.bindBuffer( gl.ARRAY_BUFFER, this. texBuffer );
        gl.bufferData( gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);


        // normals
        gl.bindBuffer( gl.ARRAY_BUFFER, this. normBuffer );
        gl.bufferData( gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW); // 3D 
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		// Set the uniform parameter(s) of the vertex shader
		gl.useProgram(this.prog);
        gl.uniform1i( this.swapAx, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{

        gl.useProgram(this.prog);// bind prog



        gl.uniformMatrix4fv( this.mvp, false, matrixMVP );// set uniform var for all vertices

        
        // TODO: w/ 2nd 2 params - transform object-space vertex positions and normals to the
        //                         camera space, where you can perform shading.

       

        // set uniforms, transform the vertP and norms in shaders
        gl.uniformMatrix4fv( this.mmv, false, matrixMV );
        gl.uniformMatrix3fv( this.mn, false, matrixNormal);


        //TODO: compute and set uniform diffuse and specular comps kDif and kShin


        // vertex positons attribute
        gl.bindBuffer( gl.ARRAY_BUFFER, this.vertBuffer );
		gl.vertexAttribPointer( this.vertP, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.vertP );

        // texture coordinates attribute
		gl.bindBuffer( gl.ARRAY_BUFFER, this.texBuffer );
		gl.vertexAttribPointer( this.txc, 2, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.txc );


        // normals attribute
        gl.bindBuffer( gl.ARRAY_BUFFER, this.normBuffer );
		gl.vertexAttribPointer( this.norm, 3, gl.FLOAT, false, 0, 0 );
		gl.enableVertexAttribArray( this.norm );
        

       
        // draw
        gl.clear( gl.COLOR_BUFFER_BIT );
        gl.useProgram(this.prog);
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
	    // Bind the texture
		gl.activeTexture( gl.TEXTURE0 );
		
        gl.bindTexture( gl.TEXTURE_2D, this.myTex );

		// Set the texture image data 
		gl.texImage2D( gl.TEXTURE_2D,         //2d texture
		               0,                     // Mipmap level 0
		               gl.RGB,                // Internal Format   
		               gl.RGB,                // Format
		               gl.UNSIGNED_BYTE,      // type
		               img                    // texture image
		               );

		gl.generateMipmap( gl.TEXTURE_2D );  
		     	
	
		// set some uniform parameter of the fragment shader, so that it uses the texture.
        gl.useProgram( this.prog ); // bind program
        gl.uniform1i( this.sampler, 0 );

        gl.uniform1i( this.loadedImg, true);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		// set the uniform parameter for show in the fragment shader to specify if it should use the texture.
		gl.useProgram(this.prog);
		gl.uniform1i(this.showTex, show);
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the light direction.
		                
        // TODO: check if done
        gl.useProgram(this.prog);
        gl.uniform3f( this.lightdir, x, y, z);
		            
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		// [TO-DO] set the uniform parameter(s) of the fragment shader to specify the shininess.
		                
		 //TODO: check if done                  
		gl.useProgram(this.prog);
        gl.uniform1f(this.shiny, shininess);
	}
}

// Vertex Shader
var meshVS = `
    
    
	attribute vec3 vertP; //position of vertex

    attribute vec2 txc;

    

    // uniform for swap
    uniform bool swapAx;
	
	uniform mat4 mvp; // 4x4 projection matrix     

	uniform mat4 mmv;
	uniform mat3 mn; 

	varying vec2 texCoord;   
	varying vec3 viewDir;
	varying vec3 normal;
	varying vec3 pos;

	attribute vec3 norm;//  new attrib 
	


	void main()
	{

    // bring into view space
    // vertP' = matrixMV * vertP   
    // norms' = matrixNormal * norms

       
		


		if(swapAx)
		{
             gl_Position = mvp * vec4(vertP.x,vertP.z, vertP.y, 1); // swapped y and z coord

             normal = mn * vec3(norm.x,norm.z,norm.y);// bring into view space
             pos = (mmv * vec4(vertP.x,vertP.z,vertP.y,1)).xyz;
        }
        else // no swap
        {
        	 gl_Position = mvp * vec4(vertP, 1); // vec4(x, y, z, 1) homogenous coordinates

        	 normal = mn * vec3(norm.x,norm.y,norm.z);// bring into view space
        	 pos = (mmv * vec4(vertP.x,vertP.y,vertP.z,1)).xyz;
        }
          
        texCoord = txc;


        viewDir = normalize(-pos);// set view direction


	}
`;


// Fragment Shader
var meshFS = `
	precision mediump float;

	uniform sampler2D tex;

    varying vec2 texCoord;
    varying vec3 normal;
    varying vec3 viewDir;
   // varying vec3 pos;

    // uniform for show texture
    uniform bool showTex;

    uniform bool loadedImg;
   

    // uniform to specify light direction - this is already is the view space
    uniform vec3 lightdir;


    // material properties
    uniform float shiny; 


	void main()
	{		


        //normalize normals
       //normalize(normal); // not sure if necessary

     

        // TODO: implemnt shading here using Blinn material model
        // shade everything in the view space

       // C = I*(cosTheta*kDif + kSpec*(cosPhi^shiny)) (optional)+ Ia*Ka (ambient light)

                                         //If p' is a vector of position values for a vertex that has 
                                         // been transformed from the model to view/camera space, the 
                                         // view direction can be obtained from normalize(-p').

// ***--- Implementation ----*** 

       vec4 lightI = vec4(1,1,1,1);// (taken as white in RGB, stated in instructions)
    

       // The diffuse and specular color coefficients (Kd and Ks) should be taken as white, vec4(1,1,1,1) 
       // If showTexture is set and setTexture is called, the diffuse coefficient (Kd) should be replaced by the texture value.

       vec4 kDif = vec4(1,1,1,1);
       vec4 kSpec = vec4(1,1,1,1);

       if(loadedImg && showTex)
       {
            kDif = texture2D(tex, texCoord);
       }

     
       vec3 normnorm = normalize(normal);
       vec3 normlight = normalize(lightdir);
       vec3 normview = normalize(viewDir);


      float cosTheta = dot(normnorm, normlight); // n . w , n is surface normal, w is light direction


      cosTheta = max(cosTheta, 0.0);

    
      vec4 diffuse = cosTheta * kDif;


      vec3 h = normalize(normlight+normview);// h = normalize(w + v) aka (w + v)/|w + v| , v is viewing direction
      
     
      vec4 specular = pow(max(dot(normnorm, h), 0.0), shiny)*kSpec; // dot(normnorm, h) is cosPhi, specular is never below 0


      if(dot(normnorm, h) < 0.0){ specular = vec4(0.0, 0.0, 0.0, 1.0); } // no specular if light is behind object
  
  
      vec4 color = lightI * (diffuse + specular);

      color.a = 1.0; //make it not transparent


      gl_FragColor = color;

	       
	}
`;