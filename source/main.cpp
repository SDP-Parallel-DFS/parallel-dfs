#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.hpp"

// READ_TYPE = 0 => C fscanf
// READ_TYPE = 1 => C++ ifstream
#define READ_TYPE 0

// Set to 1 to time each phase of the algorithm.
#define BENCHMARK 0

using namespace std;

int main(int argc, const char *argv[])
{
	try
	{
		setbuf(stdout, nullptr);
		// 1 parameter of format .gra is required.
		if (argc != 3)
		{
			cout << "Usage: " << argv[0] << " input_file output_file" << endl;
			return -1;
		}
		string graname(argv[1]);
		string outname(argv[2]);

		auto itotal = std::chrono::steady_clock::now();

#if READ_TYPE == 1
		std::ifstream ifs(graname, std::ifstream::in);
		if (!ifs.is_open())
		{
			cout << "Error: File doesn't exist." << endl;
			return -1;
		}
#else
		FILE *fp;
		if ((fp = fopen(graname.c_str(), "r")) == NULL)
		{
			cout << "Error: " << graname << " doesn't exist." << endl;
			return -1;
		}
#endif

		auto istart = std::chrono::steady_clock::now();
#if READ_TYPE == 1
		Graph gp(ifs);
		ifs.close();
#else
		Graph gp(fp);
		fclose(fp);
#endif
		gp.sortVectors();
#if BENCHMARK
		auto iend = std::chrono::steady_clock::now();
		// gp.printGraph();
		std::chrono::duration<double> ielapsed_seconds = iend - istart;
		std::cout << "Init elapsed time: " << ielapsed_seconds.count() << "s\n";
		auto start = std::chrono::steady_clock::now();
#endif
		gp.buildDT();
#if BENCHMARK
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "DT elapsed time: " << elapsed_seconds.count() << "s\n";
		auto sstart = std::chrono::steady_clock::now();
#endif
		gp.computeSubDTSize();
#if BENCHMARK
		auto send = std::chrono::steady_clock::now();
		std::chrono::duration<double> selapsed_seconds = send - sstart;
		std::cout << "subgraph size elapsed time: " << selapsed_seconds.count() << "s\n";
		auto ppstart = std::chrono::steady_clock::now();
#endif
		gp.computePrePostOrder();
		gp.computeLabels();
#if BENCHMARK
		auto ppend = std::chrono::steady_clock::now();
		std::chrono::duration<double> ppelapsed_seconds = ppend - ppstart;
		std::cout << "pre post elapsed time: " << ppelapsed_seconds.count() << "s\n";
		auto itotalend = std::chrono::steady_clock::now();
		std::chrono::duration<double> pptotal = itotalend - itotal;
		cout << "total time taken:" << pptotal.count() << endl;
#endif
		//gp.printGraph();

		FILE *out;
		if ((out = fopen(outname.c_str(), "w")) == NULL)
		{
			cout << "Error: " << outname << " can't be written." << endl;
			return -1;
		}
		gp.printNodesStatus(out);
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	return 0;
}
