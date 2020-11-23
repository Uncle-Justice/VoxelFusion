#pragma once

#define OPEN_NI
#define BINARY_DUMP_READER
#define SENSOR_DATA_READER

#define RUN_MULTITHREADED

#include "stdafx.h"

#include <vector>
#include <string>
#include <list>


#define RENDERMODE_INTEGRATE 0 
#define RENDERMODE_VIEW 1

#define X_GLOBAL_APP_STATE_FIELDS \
	X(uint, s_sensorIdx) \
	X(uint, s_windowWidth) \
	X(uint, s_windowHeight) \
	X(uint, s_integrationWidth) \
	X(uint, s_integrationHeight) \
	X(uint, s_rayCastWidth) \
	X(uint, s_rayCastHeight) \
	X(uint, s_maxFrameFixes) \
	X(uint, s_topNActive) \
	X(float, s_minPoseDistSqrt) \
	X(float, s_sensorDepthMax) \
	X(float, s_sensorDepthMin) \
	X(float, s_renderDepthMax) \
	X(float, s_renderDepthMin) \
	X(float, s_cameraIntrinsicFx) \
	X(float, s_cameraIntrinsicFy) \
	X(float, s_cameraIntrinsicCx) \
	X(float, s_cameraIntrinsicCy) \
	X(uint, s_hashNumBuckets) \
	X(uint, s_hashNumSDFBlocks) \
	X(uint, s_hashMaxCollisionLinkedListSize) \
	X(float, s_SDFVoxelSize) \
	X(float, s_SDFMarchingCubeThreshFactor) \
	X(float, s_SDFTruncation) \
	X(float, s_SDFTruncationScale) \
	X(float, s_SDFMaxIntegrationDistance) \
	X(uint, s_SDFIntegrationWeightSample) \
	X(uint, s_SDFIntegrationWeightMax) \
	X(std::string, s_binaryDumpSensorFile) \
	X(bool, s_binaryDumpSensorUseTrajectory) \
	X(bool, s_loadHashData) \
	X(std::string, s_hashDataFile) \
	X(bool, s_loadRayCastData) \
	X(std::string, s_rayCastDataFile) \
	X(float, s_depthSigmaD) \
	X(float, s_depthSigmaR) \
	X(bool, s_depthFilter) \
	X(float, s_colorSigmaD) \
	X(float, s_colorSigmaR) \
	X(bool, s_colorFilter) \
	X(vec4f, s_materialAmbient) \
	X(vec4f, s_materialSpecular) \
	X(vec4f, s_materialDiffuse) \
	X(float, s_materialShininess) \
	X(vec4f, s_lightAmbient ) \
	X(vec4f, s_lightDiffuse) \
	X(vec4f, s_lightSpecular) \
	X(vec3f, s_lightDirection) \
	X(float, s_SDFRayIncrementFactor) \
	X(float, s_SDFRayThresSampleDistFactor) \
	X(float, s_SDFRayThresDistFactor) \
	X(bool, s_integrationEnabled) \
	X(bool, s_trackingEnabled) \
	X(bool, s_garbageCollectionEnabled) \
	X(uint, s_garbageCollectionStarve) \
	X(bool, s_SDFUseGradients) \
	X(bool, s_timingsDetailledEnabled) \
	X(bool, s_timingsTotalEnabled) \
	X(uint, s_RenderMode) \
	X(bool, s_playData) \
	X(float, s_renderingDepthDiscontinuityThresLin) \
	X(float, s_remappingDepthDiscontinuityThresLin) \
	X(float, s_remappingDepthDiscontinuityThresOffset) \
	X(float, s_renderingDepthDiscontinuityThresOffset) \
	X(bool, s_bUseCameraCalibration) \
	X(uint, s_marchingCubesMaxNumTriangles) \
	X(bool, s_streamingEnabled) \
	X(vec3f, s_streamingVoxelExtents) \
	X(vec3i, s_streamingGridDimensions) \
	X(vec3i, s_streamingMinGridPos) \
	X(uint, s_streamingInitialChunkListSize) \
	X(float, s_streamingRadius) \
	X(vec3f, s_streamingPos) \
	X(uint, s_streamingOutParts) \
	X(uint, s_recordDataWidth) \
	X(uint, s_recordDataHeight) \
	X(bool, s_recordData) \
	X(bool, s_recordCompression) \
	X(std::string, s_recordDataFile) \
	X(bool, s_reconstructionEnabled) \
	X(bool, s_generateVideo) \
	X(std::string, s_generateVideoDir) \
	X(std::string, s_printTimingsDirectory) \
	X(std::string, s_printConvergenceFile) \
	X(mat4f, s_topVideoTransformWorld) \
	X(vec4f, s_topVideoCameraPose) \
	X(vec2f, s_topVideoMinMax) \
	X(uint, s_numSolveFramesBeforeExit) \
	X(std::string, s_generateMeshDir) \
	X(bool, s_generateMeshWithNormals)


#ifndef VAR_NAME
#define VAR_NAME(x) #x
#endif

#define checkSizeArray(a, d)( (((sizeof a)/(sizeof a[0])) >= d))

class GlobalAppState
{
public:

#define X(type, name) type name;
	X_GLOBAL_APP_STATE_FIELDS
#undef X

		//! sets the parameter file and reads
	void readMembers(const ParameterFile& parameterFile) {
		m_ParameterFile = parameterFile;
		readMembers();
	}

	//! reads all the members from the given parameter file (could be called for reloading)
	void readMembers() {
#define X(type, name) \
	if (!m_ParameterFile.readParameter(std::string(#name), name)) {MLIB_WARNING(std::string(#name).append(" ").append("uninitialized"));	name = type();}
		X_GLOBAL_APP_STATE_FIELDS
#undef X
 

		m_bIsInitialized = true;
	}

	void print() const {
#define X(type, name) \
	std::cout << #name " = " << name << std::endl;
		X_GLOBAL_APP_STATE_FIELDS
#undef X
	}

	static GlobalAppState& getInstance() {
		static GlobalAppState s;
		return s;
	}
	static GlobalAppState& get() {
		return getInstance();
	}


	//! constructor
	GlobalAppState() {
		m_bIsInitialized = false;
		//m_pQuery = NULL;
	}

	//! destructor
	~GlobalAppState() {
	}


	//HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice);
	void OnD3D11DestroyDevice();

	void WaitForGPU();

	Timer	s_Timer;

private:
	bool			m_bIsInitialized;
	ParameterFile	m_ParameterFile;
	//ID3D11Query*	m_pQuery;
};
