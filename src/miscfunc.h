/*
 * miscfunc
 * Date: Jun-08-2014 
 * Author : Gabriel Renaud gabriel.reno [at sign here] gmail.com
 *
 */

#ifndef miscfunc_h
#define miscfunc_h

#include <stdlib.h>
#include <vector>
#include <set>
#include <string>
#include <gzstream.h>

#include "utils.h"

#define DEBUGCOV
using namespace std;

static vector<long double> lnFactVec;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


typedef struct { 
    vector<int> seq;
    double llik;
 } hmmpath;

typedef struct { 
    int maxReadLength;
    bool isPe;
 } rgInfo;

typedef struct { 
    long double h;
    long double hLow;
    long double hHigh;
    unsigned int sites;
    bool hasConverged;
 } hResults;


typedef struct { 
    long double s[12];
 } substitutionRates;


//  model->obs
//  0  A->A 
//  1  A->C 
//  2  A->G 
//  3  A->T 
//  4  C->A 
//  5  C->C 
//  6  C->G 
//  7  C->T 
//  8  G->A 
//  9  G->C 
//  10 G->G 
//  11 G->T 
//  12 T->A 
//  13 T->C 
//  14 T->G 
//  15 T->T 

typedef struct { 
    long double s[16];
} probSubstition;

typedef struct { 
    long double gl[16];
} babdlikelihood;

typedef struct { 
    long double p[4][4];
} diNucleotideProb;


//frequency of A,C,G,T
typedef struct { 
    long double f[4];
 } alleleFrequency;

typedef struct { 
    char b;//observed base
    int bq;//base quality phred scale
    int mpq;//mapping qual
    long double probDDc;
    long double probADc;
    long double probDAc;
    long double probAAc;

    long double probDDe;
    long double probADe;
    long double probDAe;
    long double probAAe;

 } singleBase;


typedef struct { 
    string id;
    int leftCoord;//base quality phred scale
    int rightCoord;
 } region;


typedef struct { 
    string chr;
    unsigned int coord;

    char ancAllele;
    char derAllele;
    
    vector<singleBase> sitesBAMd;
    vector<singleBase> sitesBAMa;
    vector<double>     freqDerived;
 } singleSite;


//To store consensus information
typedef struct { 
    long double perror[4];
    long double phred[4];
    long double perrorC[4];
    long double phredC[4];

    char ref;
    char consensus; //for the endogenous
} PHREDgeno;


typedef struct { 

    char ref;
    char base; //for the endogenous
    int  pos;
    double q;

    double aprob;
    double cprob;
    double gprob;
    double tprob;

} logRecord;

//To store a single read
typedef struct { 
    uint8_t base;
    uint8_t qual;
    uint8_t mapq;       
    uint8_t pos5p;       
    uint8_t lengthF;             
    /* int dist5p; */
    /* int dist3p; */
    /* string name; //TO REMOVE */
    bool isrv;
} singleRead;

//To store contamination likelihood
typedef struct { 
    string filename;    
    long double contaminationRate;
    long double logLike;
    //    long double logLike;
} contaminationEstimate;


typedef struct { 
    vector<singleRead> readsVec;
    /* long double mapqAvg; */
    //int cov;
    char refBase;
    //int refID;
    int posAlign;
    int avgMQ;
    int          baseC[4];
    bool skipPosition;
} positionInformation;


typedef struct { 
    long double likeBaseNoindel[4];
    long double likeBaseNoindelCont[4][4];
    long double likeBaseNoindelNoBoundary[4];           //when we do not consider bases at the ends of reads
    long double likeBaseNoindelContNoBoundary[4][4];    //when we do not consider bases at the ends of reads

    int  covPerBase[4];
    int  covPerBaseNoBoundary[4];

    long double mapqAvg;
    
    int numDel;
    long double llikDeletion;
    long double llikNoDeletion;

    long double llikDeletionBoth;
    long double llikDeletionCont;
    long double llikDeletionEndo;
    long double llikDeletionNone;

    set<string> allInserts;

    vector<string> insertionRight;
    map< pair<string,string> , long double> insertion2loglikeEndoCont; //key is (endo ins,cont ins) to log likelihood

    map<string,int> insertion2count;
    map<string,long double> insertion2loglike;
    //map<string,long double> insertion2loglikeCont;

    int cov;
} singlePosInfo;



void readNucSubstitionRatesFreq(const string filename,     vector<substitutionRates> & subVec);
void readNucSubstitionFreq(     const string filename,     vector<probSubstition>    & subVec);
void readDNABaseFreq(const string filename, alleleFrequency & dnaDefaultFreq);

void readIlluminaError(const string errFile,probSubstition & illuminaErrorsProb);


inline long double lnfact (unsigned int n){
    long double toreturn=0.0;
    for(unsigned int i=1;i<=n;i++){
	toreturn +=  logl( (long double)i );
    }
    return toreturn;
}

inline void initMiscFuncVariables(){
    for(double i=0;i<50;i++){
    	lnFactVec.push_back(lnfact(i) );
     }    
}

//taken from gsl/randist/multinomial.c
inline long double gsl_ran_multinomial_lnpdf (const size_t K, const long double p[], const unsigned int n[]){
  size_t k;
  unsigned int N      =   0;
  long double log_pdf = 0.0;
  long double norm    = 0.0;

  for (k = 0; k < K; k++){
      N += n[k];
  }
  
  for (k = 0; k < K; k++){
      norm += p[k];
  }
  
  log_pdf = lnfact (N);

  for (k = 0; k < K; k++){
      log_pdf -= lnfact (n[k]);
  }

  for (k = 0; k < K; k++){
      log_pdf += logl (p[k] / norm) * n[k];
  }

  return log_pdf;
}

inline long double gsl_ran_multinomial_pdf (const size_t K,const long double p[], const unsigned int n[]){
    return expl (gsl_ran_multinomial_lnpdf (K, p, n));
}


void populatedCoverateVector(      vector<long double> * cov2ProbSite, long double rateForPoissonCov, int maxcov);
void populatedCoverateVectorSingle(vector<long double> * cov2ProbSite, long double rateForPoissonCov, int maxcov);
/* void readMTConsensus(const string consensusFile,map<int, PHREDgeno> & pos2phredgeno,int & sizeGenome,vector<int> & posOfIndels); */
/* void readMTAlleleFreq(const string freqFile,	map<int, alleleFrequency> & pos2allelefreq); */


/* long double computeLL(const int                   al1Current    , */
/* 		      const int                   al2Current    ,		       */
/* 		      const vector<int>         & obsBase       , */
/* 		      const vector<long double> & probDeam1to2  , //rate of deamination from al1 to al2 */
/* 		      const vector<long double> & probDeam2to1  , //rate of deamination from al2 to al1 */
/* 		      const vector<int>         & obsQual       , */
/* 		      const long double           contRate      , */
/* 		      const int                   alContCurrent , */
/* 		      const vector<long double> & mismappingProb); */

// Returns log( exp(x)+exp(y) ), but does so without causing
// overflow or loss of precision.
/* inline double oplusl( long double x, long double y ){ */
/*     return x > y  */
/*         ? x + log1pl( expl(y-x ) )  */
/*         : y + log1pl( expl(x-y ) ) ; */
/* } */

// Returns log10( pow(10,x)+pow(10,y) ), but does so without causing
// overflow or loss of precision.
/* template <typename T> */
/* inline T oplusInit(T x,T y ){ */
/*     if( x == 0 ){ //no initialized, a log = 0 should not exist */
/* 	return y; */
/*     } */

/*     return x > y  */
/*         ? x + log1p( pow( 10, y-x ) ) / log(10) */
/*         : y + log1p( pow( 10, x-y ) ) / log(10) ; */
/* } */


#endif
