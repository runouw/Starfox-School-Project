# include <gltools/SGNode.h>

GLTools::SGNode::SGNode(){
	this->tr = new GsMat();
	this->object = NULL;
}
GLTools::SGNode::SGNode(GLObject* obj, GsMat* tr){
	this->tr = tr;
	this->object = obj;
}
void GLTools::SGNode::update(GsMat* tr){
	if (this->tr != NULL){
		delete (this->tr);
	}
	this->tr = tr;
}
void* GLTools::SGNode::addChild(void* node){
	this->children.push(node);
	return node;
}
void* GLTools::SGNode::addChild(GLObject* obj, GsMat* tr){
	SGNode* node = new SGNode(obj, tr);

	this->children.push(node);
	return node;
}
void GLTools::SGNode::draw(GsMat& tr, GsMat& pr){
	// draw the current object
	if (this->object != NULL){
		this->object->draw(*(this->tr) * tr, pr);
	}
	// draw all its children	
	for (int i = 0; i < this->children.size(); i++){
		SGNode* node = (SGNode*)(this->children[i]);
		if (node != NULL){
			node->draw(*(this->tr) * tr, pr);
		}
	}
}