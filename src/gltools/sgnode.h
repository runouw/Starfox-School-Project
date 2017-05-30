# ifndef SGNODE_H
# define SGNODE_H

# include <gltools/gltools.h>

/*
Scene graph object
*/
namespace GLTools{
	class SGNode{
	private:
		GsArray<void*> children;
		GLObject* object;
		GsMat* tr;
		uint32_t cCount;
	public:
		SGNode();
		SGNode(GLObject* obj, GsMat* tr);
		void* addChild(void*);
		void* addChild(GLObject* obj, GsMat* tr);
		void draw(GsMat& tr, GsMat& pr);
		void set(GLObject* obj, GsMat* tr);
		void update(GsMat* tr);
	};
}
# endif