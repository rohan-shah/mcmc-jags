#ifndef CODA_H_
#define CODA_H_

#include <model/MonitorControl.h>

#include <list>
#include <string>

/**
 * CODA output for monitors that have a separate value for each chain
 * This function opens up an index file and one output file with names
 * "<prefix>index.txt", "<prefix>chain1.txt" ... "<prefix>chain<nchain>.txt"
 *
 * @param mvec List of MonitorControl objects containing monitors to be 
 * written out.
 * @param prefix Prefix to be added to index and output file names.
 * @param nchain Number of chains.
 * @param warn String that will contain warning messages on exit. It is
 *        cleared on entry.
 */
void CODA(std::list<MonitorControl> const &mvec, std::string const &prefix,
	  unsigned int nchain, std::string &warn);

/**
 * CODA output for monitors that have pool values over chains.
 * This function opens up an index file "<prefix>index0.txt"
 * and one output file "<prefix>chain0.txt".
 *
 * @param mvec List of MonitorControl objects containing monitors to be 
 * written out.
 * @param Prefix to be prepended to index and output file names
 * @param warn String that will contain warning messages on exit. It is
 *        cleared on entry.
 */
void CODA0(std::list<MonitorControl> const &mvec, std::string const &prefix,
	   std::string &warn);

#endif /* CODA_H_ */
