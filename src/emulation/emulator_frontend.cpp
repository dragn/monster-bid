/*
 * =====================================================================================
 *
 *       Filename:  emulator_frontend.cpp
 *
 *    Description:  Emulator frontend implementation
 *
 *        Version:  1.0
 *        Created:  12/06/2011 01:36:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "emulator_frontend.h"

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  EmulatorFrontend
 *      Method:  EmulatorFrontend
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
EmulatorFrontend::EmulatorFrontend ()
{
	//TODO Currently only CPU emulator supported
	cpu_emulator.reset( new CpuEmulator() );	
}  /* -----  end of method EmulatorFrontend::EmulatorFrontend  (constructor)  ----- */


boost::shared_ptr<Emulator>
EmulatorFrontend::choose_emulator ()
{
	return cpu_emulator;
}		/* -----  end of method EmulatorFrontend::choose_emulator  ----- */


boost::shared_array<t_data>
EmulatorFrontend::single_run ( NetworkPtr net, boost::shared_array<t_data> input )
{
	boost::shared_ptr<Emulator> emul = choose_emulator();
	return emul->single_run( net, input );
}		/* -----  end of method EmulatorFrontend::single_run  ----- */

