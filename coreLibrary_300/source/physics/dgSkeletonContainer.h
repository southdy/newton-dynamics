/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _DG_SKELETON_CONTAINER_H__
#define _DG_SKELETON_CONTAINER_H__

#define DG_SKELETON_BASEW_UNIQUE_ID	10

#include "dgConstraint.h"

class dgDynamicBody;
class dgSkeletonContainer;
typedef void (dgApi *dgOnSkeletonContainerDestroyCallback) (dgSkeletonContainer* const me);

class dgSkeletonContainer
{
	public:
	DG_MSC_VECTOR_ALIGMENT
	class dgForcePair
	{
		public:
		dgSpatialVector m_joint;
		dgSpatialVector m_body;
	} DG_GCC_VECTOR_ALIGMENT;


	class dgSkeletonGraph;

	DG_CLASS_ALLOCATOR(allocator)
	dgSkeletonContainer(dgWorld* const world, dgDynamicBody* const rootBody);
	~dgSkeletonContainer();

	void Finalize ();
	dgWorld* GetWorld() const; 
	dgInt32 GetId () const {return m_id;}
	dgInt32 GetJointCount () const {return m_nodeCount - 1;}
	dgSkeletonGraph* AddChild (dgBody* const child, dgBody* const parent);
	void AddJointList (dgInt32 count, dgBilateralConstraint** const array);
	void SetDestructorCallback (dgOnSkeletonContainerDestroyCallback destructor);
	void InitMassMatrix (const dgJointInfo* const jointInfoArray, dgJacobianMatrixElement* const matrixRow) const;
//	dgFloat32 CalculateJointForce (dgJointInfo* const jointInfoArray, const dgBodyInfo* const bodyArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow);
	void CalculateJointForce (dgJointInfo* const jointInfoArray, const dgBodyInfo* const bodyArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow);
	bool GetSolverMode () const;
	void SetSolverMode (bool hardJoint);
	
	dgSkeletonGraph* GetRoot () const;
	dgBody* GetBody(dgSkeletonGraph* const node) const;
	dgBilateralConstraint* GetParentJoint(dgSkeletonGraph* const node) const;
	dgSkeletonGraph* GetParent (dgSkeletonGraph* const node) const;
	dgSkeletonGraph* GetFirstChild (dgSkeletonGraph* const parent) const;
	dgSkeletonGraph* GetNextSiblingChild (dgSkeletonGraph* const sibling) const;

	private:
	DG_INLINE void SolveBackward (dgForcePair* const forces) const;
	DG_INLINE void SolveFoward (dgForcePair* const forces) const;
	DG_INLINE void UpdateMassMatrixLCP (const dgJointInfo* const jointInfoArray, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces) const;
	DG_INLINE void UpdateForces (dgJointInfo* const jointInfoArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, const dgForcePair* const forces) const;
	DG_INLINE void CalculateJointAccel (dgJointInfo* const jointInfoArray, const dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces) const;
	DG_INLINE void InitMassMatrixLCP (const dgJointInfo* const jointInfoArray, const dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces) const;
	
	DG_INLINE void EnumerateRowsAndInitForces (const dgJointInfo* const jointInfoArray, dgForcePair* const forces) const;

	DG_INLINE bool ValidateForcesLCP (dgJointInfo* const jointInfoArray, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces);
	void FindFirstVariablesSet (dgJointInfo* const jointInfoArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces);

	void SoveLCP (dgJointInfo* const jointInfoArray, const dgBodyInfo* const bodyArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces);
	void SoveNormal (dgJointInfo* const jointInfoArray, const dgBodyInfo* const bodyArray, dgJacobian* const internalForces, dgJacobianMatrixElement* const matrixRow, dgForcePair* const forces);


	dgSkeletonGraph* FindNode (dgDynamicBody* const node) const;
	dgSkeletonGraph* AddChild (dgDynamicBody* const child, dgDynamicBody* const parent);
	void SortGraph (dgSkeletonGraph* const root, dgSkeletonGraph* const parent, dgInt32& index);

void XXXX();
	
	static void ResetUniqueId(dgInt32 id);

	dgWorld* m_world;
	dgSkeletonGraph* m_skeleton;
	dgSkeletonGraph** m_nodesOrder;
	dgOnSkeletonContainerDestroyCallback m_destructor;
	dgInt32 m_id;
	dgInt32 m_lru;
	dgInt16 m_nodeCount;
	dgInt8 m_skeletonHardMotors;
	static dgInt32 m_uniqueID;
	static dgInt32 m_lruMarker;

	friend class dgWorld;
	friend class dgWorldDynamicUpdate;
};

#endif

