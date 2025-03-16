#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# https://developer.android.com/ndk/guides/android_mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Bullet

LOCAL_SRC_FILES := \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btDbvt.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btGhostObject.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/btUnionFind.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btBox2dShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btBoxShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btCollisionShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btCompoundShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConcaveShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConeShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btCylinderShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btEmptyShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btMiniSDF.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btSdfCollisionShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btShapeHull.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btSphereShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
    $(LOCAL_PATH)/../src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btBatchedConstraints.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/Dynamics/btRigidBody.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
    $(LOCAL_PATH)/../src/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btAlignedAllocator.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btConvexHull.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btConvexHullComputer.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btGeometryUtil.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btQuickprof.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btThreads.cpp \
    $(LOCAL_PATH)/../src/LinearMath/btVector3.cpp

LOCAL_CFLAGS :=

ifeq (armeabi-v7a,$(TARGET_ARCH_ABI))
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
else ifeq (arm64-v8a,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS +=
else ifeq (x86,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else ifeq (x86_64,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else
LOCAL_CFLAGS +=
endif

LOCAL_CFLAGS += -Wall
LOCAL_CFLAGS += -Werror=return-type

LOCAL_CFLAGS += -DBT_THREADSAFE=1
ifeq (true, $(APP_DEBUG))
LOCAL_CFLAGS += -DBT_DEBUG_MEMORY_ALLOCATIONS=1
endif

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../src

LOCAL_CPPFLAGS := 
LOCAL_CPPFLAGS += -std=c++20

include $(BUILD_STATIC_LIBRARY)

