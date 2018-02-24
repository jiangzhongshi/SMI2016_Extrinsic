
#include "geo_sur.h"
#include "readers.h"
namespace n_rf {

int Surface::Initialize(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F,
 bool isEigenInit, bool isGaussIter,
 Eigen::MatrixXd& out_field, Eigen::MatrixXd& out_normal){
    clearup();
    bool rere = false;

    rere = readVF(V, F);

    if(!rere){
        cout<<"invalid input from buffer: V, F"<<endl;
        return -1;
    }

    setparameters();
    ReScale_uniform(1.0);
    isbuilddisp = false;
    cout<<"read finish!"<<endl;


    BuildNeighborTable();
    //ReOrientFaces();
    ComputeEdgefromFace();

    ComputeEdgeLength();
    ComputeDefectAngle();
    ComputeFaceNormal();

    BuildFacesCenter();
    ComputeArea();


    BuildPickID();


    InitializeField(false,true);
    cout<<"Mesh Initialized!"<<endl;

    if(isEigenInit)InitializeFieldByLeastEigenV();
    if(isGaussIter)RunGaussSeidelIteration();
    ComputeVerticesFieldSingularity();

    if(n_vertices<5000)sparseSampling(1);
    else if(n_vertices<30000)sparseSampling(1);
    else if(n_vertices<60000)sparseSampling(2);
    else sparseSampling(3);

    reinitflag = true;
    isbuilddisp = false;

    out_field = Eigen::MatrixXd::Zero(n_vertices, 3);
    out_normal = Eigen::MatrixXd::Zero(n_vertices, 3);
    int idx = 0;
    for(int i=0; i<n_vertices; i++)
        for (int j=0; j<3; j++) {
            out_normal(i,j) = vertices_normal[idx];
            out_field(i, j) = vertices_field[idx];
            idx ++;
        }
    // if(outputfile.size()!=0){
        // writeSurfFile(outputfile,vertices,faces2vertices,vertices_field);
    // }
    return 0;




}

int Surface::Initialize(string inputfile, string outputfile, bool isEigenInit, bool isGaussIter){
    clearup();
    bool rere = false;

    rere = ReadFile(inputfile);

    if(!rere){
        cout<<"invalid input: "<<inputfile<<endl;
        return -1;
    }

    setparameters();
    ReScale_uniform(1.0);
    isbuilddisp = false;
    cout<<"read finish!"<<endl;


    BuildNeighborTable();
    //ReOrientFaces();
    ComputeEdgefromFace();

    ComputeEdgeLength();
    ComputeDefectAngle();
    ComputeFaceNormal();

    BuildFacesCenter();
    ComputeArea();


    BuildPickID();


    InitializeField(false,true);
    cout<<"Mesh Initialized!"<<endl;

    if(isEigenInit)InitializeFieldByLeastEigenV();
    if(isGaussIter)RunGaussSeidelIteration();
    ComputeVerticesFieldSingularity();

    if(n_vertices<5000)sparseSampling(1);
    else if(n_vertices<30000)sparseSampling(1);
    else if(n_vertices<60000)sparseSampling(2);
    else sparseSampling(3);

    reinitflag = true;
    isbuilddisp = false;

    BuildDisplay(infoSurfDisp(true,false,true,false,
                                          true,false,
                                          30,15,130));
    if(outputfile.size()!=0){
        SaveInterface(outputfile);
        outputDisplay(outputfile);
    }
    return 0;




}






bool Surface::ReadFile(string filename){


    string filepath;
    string modname;
    string extname;

    SplitFileName(filename,filepath,modname,extname);
    cout<<modname<<' '<<extname<<' '<<filepath<<endl;

    bool issuc = false;

    if(extname==".suf"){issuc = readSufFile(filename);}
    else issuc = readfile(filename);

    if(issuc){
        modelname = modname;
        prepath = filepath;
        ext=extname;
    }

    return issuc;
}




bool Surface::SaveInterface(string filename){


    return writeSurfFile(filename,vertices,faces2vertices,vertices_field);


}

bool Surface::outputDisplay(string filename){

    return writePLYFile(filename,display_vertices,display_faces,display_normal,display_vcolor);

}
void Surface::testSlicer(int thres){

    cout<<"thres: "<<thres<<endl;

}





}//n_rf
