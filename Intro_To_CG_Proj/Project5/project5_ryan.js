// This function takes the translation and two rotation angles (in radians) as input arguments.
// The two rotations are applied around x and y axes.
// It returns the combined 4x4 transformation matrix as an array in column-major order.
// You can use the MatrixMult function defined in project5.html to multiply two 4x4 matrices in the same format.
function GetModelViewMatrix( translationX, translationY, translationZ, rotationX, rotationY )
{
	radX = degToRadians(rotationX);
	radY = degToRadians(rotationY);
	cX = Math.cos(rotationX);
	cY = Math.cos(rotationY);
	sX = Math.sin(rotationX);
	sY = Math.sin(rotationY);
	// [TO-DO] Modify the code below to form the transformation matrix.
	var trans = [
		cY,  sY*sX,  -cX*sY, 0,
		0,   cX, sX, 0,
		sY, -cY*sX, cX*cY, 0,
		translationX, translationY, translationZ, 1
	];
	return trans;
}

//convert to Radians from Deg
function degToRadians (theta) {
  return theta * (Math.PI / 180);
}


// [TO-DO] Complete the implementation of the following class.

class MeshDrawer
{
	// The constructor is a good place for taking care of the necessary initializations.
	constructor()
	{
		this.prog = InitShaderProgram( meshVS, meshFS );	
		gl.useProgram(this.prog);	
		// transformation matrix mvp
		this.mvp = gl.getUniformLocation(this.prog, 'mvp');	
		// transformation matrix mv
		this.mv = gl.getUniformLocation(this.prog, 'mv');
		// normal matrix
		this.mn = gl.getUniformLocation(this.prog, 'mn');		
		// swap xy
		this.swap = gl.getUniformLocation(this.prog, 'swap');
		// show tex
		this.show = gl.getUniformLocation(this.prog, 'show');	
		// shininess
		this.shine = gl.getUniformLocation(this.prog, 'shine');	
		//light direction
		this.lDir = gl.getUniformLocation(this.prog, 'lDir');	
		// vertex buffer	
		this.vBuf = gl.createBuffer();
		this.v = gl.getAttribLocation( this.prog, 'v' );
		// texture coord buffer
		this.tcBuf = gl.createBuffer();
		this.tc = gl.getAttribLocation( this.prog, 'tc' );
		// normal buffer
		this.nBuf = gl.createBuffer();
		this.n = gl.getAttribLocation( this.prog, 'n' );

        //initialize values
		gl.uniform1i(this.show, true);
		gl.uniform1i(this.swap, false);
		gl.uniform1f(this.shine, 100);
		gl.uniform3fv(this.lDir, [0, 0, 0]);
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
		this.numTriangles = vertPos.length / 3;
        
		//bind and update vertPos vals to buffer
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vBuf);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertPos), gl.STATIC_DRAW);

		//bind and update texCoord vals to buffer
		gl.bindBuffer(gl.ARRAY_BUFFER, this.tcBuf);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(texCoords), gl.STATIC_DRAW);

		//bind and update texCoord vals to buffer
		gl.bindBuffer(gl.ARRAY_BUFFER, this.nBuf);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normals), gl.STATIC_DRAW);
	}
	
	// This method is called when the user changes the state of the
	// "Swap Y-Z Axes" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	swapYZ( swap )
	{
		gl.useProgram(this.prog);
		gl.uniform1i(this.swap, swap);
	}
	
	// This method is called to draw the triangular mesh.
	// The arguments are the model-view-projection transformation matrixMVP,
	// the model-view transformation matrixMV, the same matrix returned
	// by the GetModelViewProjection function above, and the normal
	// transformation matrix, which is the inverse-transpose of matrixMV.
	draw( matrixMVP, matrixMV, matrixNormal )
	{
		// [TO-DO] Complete the WebGL initializations before drawing
        gl.useProgram(this.prog);
        //for vertex Pos
		gl.bindBuffer(gl.ARRAY_BUFFER, this.vBuf);
		gl.vertexAttribPointer(this.v, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray( this.v );
		//link with MVP matrix
		gl.uniformMatrix4fv(this.mvp, false, matrixMVP);
		//link with MV matrix
		gl.uniformMatrix4fv(this.mv, false, matrixMV);
		//link with N matrix
		gl.uniformMatrix3fv(this.mn, false, matrixNormal);
		//for texture Coords
	    gl.bindBuffer(gl.ARRAY_BUFFER, this.tcBuf);
		gl.vertexAttribPointer(this.tc, 2, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray( this.tc );
        //for normals
		gl.bindBuffer(gl.ARRAY_BUFFER, this.nBuf);
		gl.vertexAttribPointer(this.n, 3, gl.FLOAT, false, 0, 0);
		gl.enableVertexAttribArray( this.n );
		// estimate the curve by drawing line segments between calculated verts
		gl.drawArrays( gl.TRIANGLES, 0, this.numTriangles );
	}
	
	// This method is called to set the texture of the mesh.
	// The argument is an HTML IMG element containing the texture data.
	setTexture( img )
	{
	    // [TO-DO] Bind the texture
		const texture = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, texture);
		// You can set the texture image data using the following command.
		gl.texImage2D( gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, img );
		gl.generateMipmap(gl.TEXTURE_2D);
		// [TO-DO] Now that we have a texture, it might be a good idea to set
		// some uniform parameter(s) of the fragment shader, so that it uses the texture.
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.REPEAT);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.REPEAT);
	}
	
	// This method is called when the user changes the state of the
	// "Show Texture" checkbox. 
	// The argument is a boolean that indicates if the checkbox is checked.
	showTexture( show )
	{
		gl.useProgram(this.prog);
		gl.uniform1i(this.show, show);
	}
	
	// This method is called to set the incoming light direction
	setLightDir( x, y, z )
	{
		gl.useProgram(this.prog);
	    gl.uniform3fv(this.lDir, [x, y, z]);
	}
	
	// This method is called to set the shininess of the material
	setShininess( shininess )
	{
		gl.useProgram(this.prog);
		gl.uniform1f(this.shine, shininess);
	}
}

// Vertex Shader
var meshVS = `
	attribute vec3 v;
	attribute vec2 tc;
	attribute vec3 n;
	uniform mat4 mvp;
	uniform bool swap;
	varying vec2 texC;
	varying vec3 norm;
	varying vec3 pos;
	void main()
	{
		if(swap){
			gl_Position = mvp * vec4(v[0],v[2],v[1],1);
		}
		else{
			gl_Position = mvp * vec4(v,1);
		}
		texC=tc;
		norm = n;
		pos=v;
	}
`;

// Fragment Shader
var meshFS = `
    precision mediump float;
    uniform sampler2D tex;
    varying vec2 texC;
    uniform mat4 mv;
    uniform mat3 mn;
    varying vec3 norm;
    uniform vec3 lDir;
    uniform float shine;
    uniform bool show;
    varying vec3 pos;
	void main()
	{
		float zero = float(0);
		vec4 viewFour = mv * vec4(pos, 1);
		vec3 view = normalize( float(-1) * vec3(viewFour[0],viewFour[1],viewFour[2]));
		vec4 intensity = vec4(1,1,1,1);
		vec4 kd = vec4(1,1,1,1);
		vec4 ks = vec4(1,1,1,1);
		
		vec4 ambient = vec4(0.05,0.05,0.05,1);

		vec3 viewPlusLight = normalize(lDir) + view;
		vec3 hlf = normalize(viewPlusLight / length(viewPlusLight));

        vec3 normCam = normalize(mn * norm);

        float cosT = dot(normCam, lDir);
        float cosP = dot(normCam, hlf);
        
		if(show){
			kd = texture2D(tex, texC);
		}
		gl_FragColor = (intensity * max(zero, cosT) * (kd + ks * pow( max(zero, cosP), shine)/cosT)) + ambient * kd;
	}
`;
	 