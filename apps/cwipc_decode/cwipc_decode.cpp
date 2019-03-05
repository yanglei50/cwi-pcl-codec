#include <iostream>
#include <fstream>

#include <pcl/point_cloud.h>
#include <pcl/io/ply_io.h>

#include "cwipc_util/api_pcl.h"
#include "cwipc_codec/api.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "compressedfile.cwicpc pointcloudfile.ply" << std::endl;
        return 2;
    }
    //
    // Read compressed file
    //
    std::ifstream input(argv[1]);
	std::stringstream inputBuffer;
	inputBuffer << input.rdbuf();
	std::cerr << "Read " << input.tellg() << " compressed bytes." << std::endl;
    //
    // Uncompress
    //
     cwipc_encoder_params param;
	param.num_threads = 1;
	param.do_inter_frame = false;
	param.gop_size = 1;
	param.exp_factor = 0;
	param.octree_bits = 7;
	param.color_bits = 8;
	param.jpeg_quality = 85;
	param.macroblock_size = 16;
    cwipc_codec encoder(param);
	uint64_t timeStamp;
    cwipc_pcl_pointcloud pc = new_cwipc_pcl_pointcloud();
	if ( encoder.decompress_from_stream(pc, inputBuffer, timeStamp) < 0) {
        std::cerr << argv[0] << ": Error decoding pointcloud from " << argv[1] << std::endl;
        return 1;
	}
	std::cerr << "Decoded successfully, " << pc->size() << " points." << std::endl;
    //
    // Save pointcloud file
    //
    pcl::PLYWriter writer;
	writer.write(argv[2], *pc);
    return 0;
}
