//#include "model.h"
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

////Model::Model()
////{

////}

//void loadModel(string path)
//{
//    Assimp::Importer import;
//    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

//    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//    {
//        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
//        return;
//    }
//    directory = path.substr(0, path.find_last_of('/'));

//    processNode(scene->mRootNode, scene);
//}

//void Draw(Shader &shader)
//{
//    for(unsigned int i = 0; i < meshes.size(); i++)
//        meshes[i].Draw(shader);
//}
