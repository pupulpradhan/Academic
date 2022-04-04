
/**
 * @file functions to compute model/view/projection matrices
 *
 * @version 2021/04/01

 */

/**
 * MVPmat
 *
 * @class MVPmat
 * @classdesc Class for holding and computing model/view/projection matrices.
 *
 * @param  {DisplayParameters} dispParams    display parameters
 */
var MVPmat = function ( dispParams ) {

	// Alias for accessing this from a closure
	var _this = this;


	// A model matrix
	this.modelMat = new THREE.Matrix4();

	// A view matrix
	this.viewMat = new THREE.Matrix4();

	// A projection matrix
	this.projectionMat = new THREE.Matrix4();


	var topViewMat = new THREE.Matrix4().set(
		1, 0, 0, 0,
		0, 0, - 1, 0,
		0, 1, 0, - 1500,
		0, 0, 0, 1 );

	/* Functions */

	// A function to compute a model matrix based on the current state
	//
	// INPUT
	// state: state of StateController
	function computeModelTransform( state ) {

		/* TODO (2.1.1.3) Matrix Update / (2.1.2) Model Rotation  */

		var matrixT =  new THREE.Matrix4().makeTranslation(state.modelTranslation.x,state.modelTranslation.y,state.modelTranslation.z);
		if(state.modelRotation.x != undefined && state.modelRotation.y != undefined){
		var matrixRY = new THREE.Matrix4().makeRotationY(state.modelRotation.x);
		var matrixRX = new THREE.Matrix4().makeRotationX(state.modelRotation.y);
		var matrixR = matrixRX.multiply(matrixRY);
		return matrixT.multiply(matrixR);
	}
	else return matrixT;
	}

	// A function to compute a view matrix based on the current state
	//
	// NOTE
	// Do not use lookAt().
	//
	// INPUT
	// state: state of StateController
	function computeViewTransform( state ) {

		/* TODO (2.2.3) Implement View Transform */
		var zUnit = new THREE.Vector3(state.viewerTarget.x - state.viewerPosition.x,state.viewerTarget.y - state.viewerPosition.y, state.viewerTarget.z - state.viewerPosition.z).negate().normalize();
		var xUnit = new THREE.Vector3(0,1,0);
		xUnit.cross(zUnit);
		var yUnit = new THREE.Vector3();
		yUnit.crossVectors(zUnit,xUnit);
		var matrixT = new THREE.Matrix4().makeTranslation(-state.viewerPosition.x, -state.viewerPosition.y, -state.viewerPosition.z);
		var matrixR = new THREE.Matrix4().set(
			xUnit.x,xUnit.y,xUnit.z,0,
			yUnit.x,yUnit.y,yUnit.z,0,
			zUnit.x,zUnit.y,zUnit.z,0,
			0,0,0,1);
			return matrixR.multiply(matrixT);
	}

	// A function to compute a perspective projection matrix based on the
	// current state
	//
	// NOTE
	// Do not use makePerspective().
	//
	// INPUT
	// Notations for the input is the same as in the class.
	function computePerspectiveTransform(
		left, right, top, bottom, clipNear, clipFar ) {

		/* TODO (2.3.1) Implement Perspective Projection */

		return new THREE.Matrix4().set(
			(2*clipNear)/(right-left), 0, (right+left)/(right-left), 0,
			0, (2*clipNear)/(top-bottom), (top+bottom)/(top-bottom), 0,
			0, 0, - (clipFar + clipNear)/(clipFar - clipNear) , - (2*clipNear*clipFar)/(clipFar - clipNear),
			0, 0, - 1, 0 );

	}

	// A function to compute a orthographic projection matrix based on the
	// current state
	//
	// NOTE
	// Do not use makeOrthographic().
	//
	// INPUT
	// Notations for the input is the same as in the class.
	function computeOrthographicTransform(
		left, right, top, bottom, clipNear, clipFar ) {

		/* TODO (2.3.2) Implement Orthographic Projection */

		return new THREE.Matrix4().set(
			2/(right-left), 0, 0, -(right+left)/(right-left),
			0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom),
			0, 0, 2/(clipNear-clipFar), (clipNear+clipFar)/(clipNear-clipFar),
			0, 0, 0, 1);

	}

	// Update the model/view/projection matrices
	// This function is called in every frame (animate() function in render.js).
	function update( state ) {

		// Compute model matrix
		this.modelMat.copy( computeModelTransform( state ) );

		// Use the hard-coded view and projection matrices for top view
		if ( state.topView ) {

			this.viewMat.copy( topViewMat );

			var right = ( dispParams.canvasWidth * dispParams.pixelPitch / 2 )
				* ( state.clipNear / dispParams.distanceScreenViewer );

			var left = - right;

			var top = ( dispParams.canvasHeight * dispParams.pixelPitch / 2 )
				* ( state.clipNear / dispParams.distanceScreenViewer );

			var bottom = - top;

			this.projectionMat.makePerspective( left, right, top, bottom, 1, 10000 );

		} else {

			// Compute view matrix
			this.viewMat.copy( computeViewTransform( state ) );

			// Compute projection matrix
			if ( state.perspectiveMat ) {

				var right = ( dispParams.canvasWidth * dispParams.pixelPitch / 2 )
				* ( state.clipNear / dispParams.distanceScreenViewer );

				var left = - right;

				var top = ( dispParams.canvasHeight * dispParams.pixelPitch / 2 )
				* ( state.clipNear / dispParams.distanceScreenViewer );

				var bottom = - top;

				this.projectionMat.copy( computePerspectiveTransform(
					left, right, top, bottom, state.clipNear, state.clipFar ) );

			} else {

				var right = dispParams.canvasWidth * dispParams.pixelPitch / 2;

				var left = - right;

				var top = dispParams.canvasHeight * dispParams.pixelPitch / 2;

				var bottom = - top;

				this.projectionMat.copy( computeOrthographicTransform(
					left, right, top, bottom, state.clipNear, state.clipFar ) );

			}

		}

	}



	/* Expose as public functions */

	this.computeModelTransform = computeModelTransform;

	this.computeViewTransform = computeViewTransform;

	this.computePerspectiveTransform = computePerspectiveTransform;

	this.computeOrthographicTransform = computeOrthographicTransform;

	this.update = update;

};
