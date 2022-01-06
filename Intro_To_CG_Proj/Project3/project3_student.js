// [TODO] Complete the implementation of the following class and the vertex shader below.

class CurveDrawer {

    constructor() {
        this.prog = InitShaderProgram(curvesVS, curvesFS);
        // [TODO] Other initializations should be done here.
        // [TODO] This is a good place to get the locations of attributes and uniform variables.
       
        
        // Get the ids of the uniform variables in the shaders
        this.mvp = gl.getUniformLocation(this.prog, "mvp");
        this.glp0 = gl.getUniformLocation(this.prog, "p0");
        this.glp1 = gl.getUniformLocation(this.prog, "p1");
        this.glp2 = gl.getUniformLocation(this.prog, "p2");
        this.glp3 = gl.getUniformLocation(this.prog, "p3");
        // Get the ids of the vertex attributes in the shaders
        this.vertPos = gl.getAttribLocation(this.prog, "pos");

        this.p0 = [0, 0];
        this.p1 = [0, 0];
        this.p2 = [0, 0];
        this.p3 = [0, 0];


        // Initialize the attribute buffer
        this.steps = 100;
        // var tv = [];
        // for (var i = 0; i < this.steps; ++i) {
        //     tv.push(i / (this.steps - 1));
        // }
        // [TODO] This is where you can create and set the contents of the vertex buffer object
        // for the vertex attribute we need.


        // Create the vertex buffer object
        this.buffer = gl.createBuffer();
        // We are not filling the contents of the buffer here,
        // because we will put the control points into this buffer.
    }

    setViewport(width, height) {
        // [TODO] This is where we should set the transformation matrix.
        // [TODO] Do not forget to bind the program before you set a uniform variable value.
        gl.useProgram(this.prog);	// Bind the program
        // Compute the orthographic projection matrix and send it to the shader
        // (column major orer)
        var trans = [ 2/width,0,0,0,  
                      0,-2/height,0,0, 
                      0,0,1,0, 
                      -1,1,0,1 ];
        gl.uniformMatrix4fv(this.mvp, false, trans);
    }

    updatePoints(points) {
        // [TODO] The control points have changed, we must update corresponding uniform variables.
        // [TODO] Do not forget to bind the program before you set a uniform variable value.
        // [TODO] We can access the x and y coordinates of the i^th control points using
        // var x = pt[i].getAttribute("cx");
        // var y = pt[i].getAttribute("cy");
        this.p0 = this.readPoint(points, 0);
        this.p1 = this.readPoint(points, 1);
        this.p2 = this.readPoint(points, 2);
        this.p3 = this.readPoint(points, 3);
        gl.useProgram(this.prog);
        // const glp0 = gl.getUniformLocation(this.prog, "p0");
        // const glp1 = gl.getUniformLocation(this.prog, "p1");
        // const glp2 = gl.getUniformLocation(this.prog, "p2");
        // const glp3 = gl.getUniformLocation(this.prog, "p3");
        // FIXME why does it work without these statements???
        // gl.uniform2fv(this.glp0, this.p0);
        // gl.uniform2fv(this.glp1, this.p1);
        // gl.uniform2fv(this.glp2, this.p2);
        // gl.uniform2fv(this.glp3, this.p3);

        const positionsToDraw = [];
        for (let i = 0; i < this.steps; i++) {
            const tStep = i / (this.steps - 1);
            const pointToDraw = this.calculatePointOnBezierCurve(tStep);
            positionsToDraw.push(pointToDraw);
        }

        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.bufferData(gl.ARRAY_BUFFER, this.convertToFloat32Array(positionsToDraw), gl.STATIC_DRAW);
    }

    readPoint(points, i) {
        return [points[i].getAttribute("cx"), points[i].getAttribute("cy")];
    }

    convertToFloat32Array(positionsArray) {
        const flattenPositions = positionsArray.flatMap((pos) => [pos[0], pos[1]]);
        return new Float32Array(flattenPositions);
    }

    draw() {
        // [TODO] This is where we give the command to draw the curve.
        // [TODO] Do not forget to bind the program and set the vertex attribute.
        // Draw the line segments
        gl.useProgram(this.prog);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer);
        gl.vertexAttribPointer(this.vertPos, 2, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(this.vertPos);
        gl.drawArrays(gl.LINE_STRIP, 0, this.steps);
    }

    calculatePointOnBezierCurve(t) {
        // my exact implementation of the formula was helped by this blog post: https://webglfundamentals.org/webgl/lessons/webgl-3d-geometry-lathe.html
        const invT = (1 - t);
        const p = 
            this.addVec2(
                this.addVec2(
                    this.multVec2(this.p0, (invT**3)),  // + 
                    this.multVec2(this.p1, (3 * t * invT**2))),  // +
                this.addVec2(
                    this.multVec2(this.p2, (3 * invT * t**2)), // +
                    this.multVec2(this.p3, (t**3)))
            );
        return p;
    }

    multVec2(vec, scalar) {
        return [vec[0] * scalar, vec[1] * scalar];
    }

    addVec2(first, second) {
        return [first[0] + second[0], first[1] + second[1]];
    }
}

// Vertex Shader
var curvesVS = `
    attribute vec2 pos;  // FIXME idk why i need this pos
    uniform mat4 mvp;
    // FIXME idk why i DON'T need any of these
    attribute float t;
    uniform vec2 p0;
    uniform vec2 p1;
    uniform vec2 p2;
    uniform vec2 p3;
    void main()
    {
        // FIXME i did like nothing here but it still works
        // [TODO] Replace the following with the proper vertex shader code
        gl_Position = mvp * vec4(pos,0,1);
    }
`;

// Fragment Shader
var curvesFS = `
    precision mediump float;
    void main()
    {
        gl_FragColor = vec4(1,0,0,1);
    }
`;