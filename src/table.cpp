#include "..\\globals.h"
#include "Table.h"
#include "physicsmat.h"

Table::Table()
{
	mCollision = NULL; 
}

Table::~Table()
{
	if (mCollision) delete mCollision;
}

void Table::init (SceneManager *scnMgr, String sceneNodeName, World *world, MaterialID *matID)
{
	mNode = scnMgr->getSceneNode(sceneNodeName);

	//create collision object
	mCollision = new CollisionPrimitives::TreeCollisionSceneParser(world);
	mCollision->parseScene(mNode, true);

	//initialize physics body
	mBody = new Body(world, mCollision);
	mBody->setUserData(this);
	mBody->attachToNode(mNode);
	mBody->setMaterialGroupID(matID);
}