# Voxel Fusion

This is an ubuntu porting project for [https://github.com/niessner/BundleFusion](https://github.com/niessner/BundleFusion), a GPU-based 3D reconstruction method.

The main part of BundleFusion is discarded for personnal reason. Now this project is just about 3D reconstruction from raw RGBD and pose data.

```
@article{dai2017bundlefusion,
  title={BundleFusion: Real-time Globally Consistent 3D Reconstruction using On-the-fly Surface Re-integration},
  author={Dai, Angela and Nie{\ss}ner, Matthias and Zoll{\"o}fer, Michael and Izadi, Shahram and Theobalt, Christian},
  journal={ACM Transactions on Graphics 2017 (TOG)},
  year={2017}
}
```

## Installation

This code is tested under ubuntu16.04/GCC7/CUDA10.1 (GPU: 1060Ti).

Requirements:

* CMake
* Eigen 3.1.0
* NVIDIA CUDA /10.+
* OpenCV3.5.1

## Compiling

```
mkdir build && cd build
cmake -DVISUALIZATION=OFF ..
make -j8
```

We use -DVISUALIZATION=OFF/ON to switch visualization plug.

## Usage

* Download datasets from BundleFusion project mainpage [http://graphics.stanford.edu/projects/bundlefusion/](http://graphics.stanford.edu/projects/bundlefusion/) and unzip it.
* Run Commands:

```
cd build
./bundle_fusion_example ../zParametersDefault.txt ../zParametersBundlingDefault.txt /PATH/TO/dataset/office2 /PATH/TO/output
```

The docker image is also provided. Following the code below:

```shell
## 下载镜像
sudo docker push 873292889/voxelfusion

## 进入工作目录
cd /WORK_DIR/

## 创建工作文件夹以及数据文件夹
mkdir VoxelFusion && cd VoxelFusion && mkdir data && mkdir output

## 获取参数文件（和原来的BundleFusion的参数文件略有不同，如果内参改变了要在里面进行相应改动）
wget https://gitee.com/Uncle-Justice/BundleFusion_Ubuntu_Pangolin/blob/master/zParametersDefault.txt 
wget https://gitee.com/Uncle-Justice/BundleFusion_Ubuntu_Pangolin/blob/master/zParametersBundlingDefault.txt

## 准备数据集（.jpg+.png+.txt， 不需要转成.sens文件）
cp -r /DATASET_DIR ./data

## 运行nvidia-docker
sudo nvidia-docker run -v ./VoxelFusion:/tmp  --rm -u root  873292889/voxelfusion voxelfusion /tmp/zParametersDefault.txt  /tmp/zParametersBundlingDefault.txt /tmp/data /tmp/output
```

## Result

We provide a reconstruction result of dataset [office2](http://graphics.stanford.edu/projects/bundlefusion/data/office2/office2.zip) with Google Drive: [https://drive.google.com/file/d/121rR0_6H_xTpsSsYAHIHV_sZqJjHdN5R/view?usp=sharing](https://drive.google.com/file/d/121rR0_6H_xTpsSsYAHIHV_sZqJjHdN5R/view?usp=sharing)

## Remark

Pangolin is used in the original project but discarded here. Not only because visualization is not much needed in my application but Pangolin remains unstable in the project.