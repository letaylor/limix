/*
 * genotype.h
 *
 *  Created on: May 16, 2013
 *      Author: stegle
 */

#ifndef GENOTYPE_H_
#define GENOTYPE_H_

#include "limix/types.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "dataframe.h"
#include <string>
#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
//#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>


//namespace io = boost::iostreams;


namespace limix {

class AGenotypeContainer;
class CTextfileGenotypeContainer;



/*CGenotypeBlock
 * In-memory representation of a genotype structure, which is also a container
 */
class CGenotypeBlock : public CMemDataFrame<MatrixXd> //,public AGenotypeContainer
{
	friend class AGenotypeContainer;
	friend class CTextfileGenotypeContainer;
protected:
	PVectorXi pos;
	PVectorXs chrom;
	virtual void resizeMatrices(muint_t num_rows, muint_t num_columns);

public:
	CGenotypeBlock();
	CGenotypeBlock(const CGenotypeBlock& copy);
	CGenotypeBlock(PMatrixXd geno, PVectorXs chrom, PVectorXi pos,PVectorXs sampleIDs,PVectorXs snpIDs);
	virtual ~CGenotypeBlock();

	//virtual functions
	//Read access functions
	virtual void agetPosition(VectorXi* out) const throw(CGPMixException);
	virtual PVectorXi getPosition() const throw(CGPMixException);
	virtual void agetChromosome(VectorXs* out) const throw(CGPMixException);
	virtual PVectorXs getChromosome() const throw(CGPMixException);
};
typedef sptr<CGenotypeBlock> PGenotypeBlock;



/*
 * Abstract container of genotype data
 */
class AGenotypeContainer
{
protected:
	std::string filter_chrom;
	muint_t filter_start,filter_stop;
	bool inline check_SNP(std::string snp_chrom,muint_t snp_pos)
	{
		if (this->filter_chrom=="NAN")
			return true;
		if(snp_chrom!=filter_chrom)
			return false;
		return (snp_pos>=filter_start) && (snp_pos<filter_stop);
	}

public:
	AGenotypeContainer()
	{
		filter_start = -1;
		filter_stop = -1;
		filter_chrom = "NAN";
	};

	virtual ~AGenotypeContainer()
	{};
	//set filter of genotype class
	virtual void setFilter(std::string chrom, muint_t start,muint_t stop)
	{
		this->filter_chrom = chrom;
		this->filter_start = start;
		this->filter_stop = stop;
	}

	//virtual functions
	virtual PGenotypeBlock read(mint_t num_snps=-1) throw (CGPMixException) = 0;
};
typedef sptr<AGenotypeContainer> PGenotypeContainer;





/* Common class to read from text files.
 * supports .gem, .vcf and .plink
 */
class CTextfileGenotypeContainer : public AGenotypeContainer
{
protected:
	//file format
	enum { GEN, VCF, BED } file_format;
	//buffersize for unlimitted reading
	muint_t buffer_size;
	//stream for incoming files using the boost library
	boost::iostreams::filtering_istream in_stream;
	//filename
	std::string in_filename;
	bool is_open;
	//open files
	void openFile();
	void read_header_GEN();
	PGenotypeBlock read_GEN(mint_t num_snps) throw (CGPMixException);
public:
	CTextfileGenotypeContainer(const std::string& filename);
	virtual ~CTextfileGenotypeContainer();

	//virtual functions
	PGenotypeBlock read(mint_t num_snps=-1) throw (CGPMixException);
};
typedef sptr<CTextfileGenotypeContainer> PTextfileGenotypeContainer;


} //end: namespace limix

#endif /* GENOTYPE_H_ */
