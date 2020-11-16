#include <iostream>
#include <string>
#include <fstream>
#include <BundleFusion.h>
#include <dirent.h>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <GlobalAppState.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h> 　
#include <sys/types.h>
#include <cstddef>
#include <string.h>
#include <stdio.h>

void mkdir_path(const std::string p_cMkdir)
{
    int isCreate = mkdir(p_cMkdir.c_str(),S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
 
    if( !isCreate )
    printf("create path:%s\n",p_cMkdir.c_str());
    else
    printf("create path failed! error code : %s \n",p_cMkdir.c_str());
}

int main ( int argc, char** argv )
{
    if ( argc != 5 )
    {
        std::cout<<"usage: ./bundle_fusion_example /path/to/zParametersDefault.txt /path/to/zParametersBundlingDefault.txt /path/to/dataset"<<std::endl;
        return -1;
    }

    std::cout<<"======================BundleFusion Example=========================" << std::endl;
    std::cout<<"==  " << std::endl;
    std::cout<<"==  This is an example usage for BundleFusion SDK interface" << std::endl;
    std::cout<<"==  Author: FangGet" << std::endl;
    std::cout<<"==  " << std::endl;
    std::cout<<"===================================================================" << std::endl;

    std::string app_config_file = "";
    std::string bundle_config_file = "";
    std::string dataset_root = "";
    std::string output_root = "";

    app_config_file = std::string ( argv[1] );
    bundle_config_file = std::string ( argv[2] );
    dataset_root = std::string ( argv[3] );
    output_root = std::string( argv[4] );
    // step 1: init all resources
    if ( !initBundleFusion ( app_config_file, bundle_config_file ) )
    {
        std::cerr<<"BundleFusion init failed, exit." << std::endl;
        return -1;
    }

    // read for bundlefusion dataset from http://graphics.stanford.edu/projects/bundlefusion/
    struct dirent *ptr;
    DIR *dir;
    dir = opendir ( dataset_root.c_str() );

    if(dir == nullptr){
        std::cerr << "dataset path not exist or empty, exit." << std::endl;
        return -1;
    }

    DIR *dir_out;
    dir_out = opendir(output_root.c_str());
    if(dir_out == nullptr){
        std::cerr << "output path not exist or empty, exit." << std::endl;
        return -1;
    }

    mkdir_path(output_root+"/scan");
    mkdir_path(output_root+"/hash");
    mkdir_path(output_root+"/raycast");

    closedir(dir_out);
    
    std::vector<std::string> filenames;
    while ( ( ptr=readdir ( dir ) ) !=nullptr )
    {
        if ( ptr->d_name[0]=='.' )
        {
            continue;
        }
        std::string filename_extension = std::string ( ptr->d_name );
        if ( filename_extension.size() < 10 )
            continue;
        if ( filename_extension.substr ( filename_extension.size() - 10, 10 ) == ".color.jpg" )
        {
            filenames.push_back ( filename_extension.substr ( 0, filename_extension.size() - 10 ) );
        }
    }
    std::sort ( filenames.begin(), filenames.end() );
    for ( auto& filename : filenames )
    {
        // std::cout<<filename<<std::endl;
        std::string rgb_path = dataset_root + "/" + filename + ".color.jpg";
        std::string dep_path = dataset_root + "/" + filename + ".depth.png";
        std::string pos_path = dataset_root + "/" + filename + ".pose.txt";
        cv::Mat rgbImage = cv::imread ( rgb_path );
        cv::Mat depthImage = cv::imread ( dep_path, cv::IMREAD_UNCHANGED );

        mat4f poseImage = mat4f::zero();
        
        std::ifstream fin(pos_path);
        fin >> poseImage;   
//         if ( ! fin){
//             cout << "文件不能打开" <<endl;
//         }
//         else
// {
// char buffer[80];  
//             // 从磁盘文件输入
// fin >> buffer;  
// // 关闭文件输入流      
        fin.close();  

        // poseImage.loadMatrixFromFile(pos_path);

        if ( rgbImage.empty() || depthImage.empty() )
        {
            std::cout<<"no image founded" << std::endl;
        }

         cv::imshow ( "rgb_image", rgbImage );
//         cv::imshow ( "depth_image", depthImage );
         char c = cv::waitKey ( 20 );

        if ( processInputRGBDFrame ( rgbImage,depthImage, poseImage ) )
        {
            std::cout<<"frame: " << filename << " added into BundleFusion." << std::endl;
        }
        else
        {
            std::cout<<"\Failed! frame: " << filename << " not added into BundleFusion." << std::endl;
        }
    }

    std::cout<<"Pangolin Viewer save mesh into file" << std::endl;
    struct timeval t;
    gettimeofday ( &t,nullptr );
    writeHashData(output_root+"/hash/hashData_"+std::to_string ( t.tv_sec )+".bin");
	writeRayCastData(output_root+"/raycast/rayCastData_"+std::to_string ( t.tv_sec )+".bin");
    saveMeshIntoFile ( output_root + "/scan/scan_" + std::to_string ( t.tv_sec ) + ".ply", true );

    deinitBundleFusion();

    return 0;
}
