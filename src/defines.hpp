#ifndef DEFINES_HPP
#define DEFINES_HPP

#include <artis/kernel/AbstractAtomicModel.hpp>
#include <artis/kernel/AbstractCoupledModel.hpp>
#include <artis/kernel/Simulator.hpp>
#include <artis/observer/Observer.hpp>
#include <artis/observer/View.hpp>
#include <artis/context/Context.hpp>
#include <artis/utils/DoubleTime.hpp>
#include <ModelParameters.hpp>

class SamaraModel;

static double NullValue = -999;

struct GlobalParameters
{ };

using Model = artis::kernel::AbstractModel < artis::utils::DoubleTime,
                                             samara::ModelParameters >;

using Trace = artis::utils::Trace < artis::utils::DoubleTime >;

using TraceElement = artis::utils::TraceElement < artis::utils::DoubleTime >;

template < typename T >
using AtomicModel = artis::kernel::AbstractAtomicModel <
    T, artis::utils::DoubleTime, samara::ModelParameters >;

template < typename T >
using CoupledModel = artis::kernel::AbstractCoupledModel <
    T, artis::utils::DoubleTime, samara::ModelParameters, GlobalParameters >;

typedef artis::observer::Observer < artis::utils::DoubleTime,
                                    samara::ModelParameters > Observer;

typedef artis::observer::View < artis::utils::DoubleTime,
                                samara::ModelParameters > View;

typedef artis::kernel::Simulator < SamaraModel,
                                   artis::utils::DoubleTime,
                                   samara::ModelParameters,
                                   GlobalParameters > SamaraSimulator;

typedef artis::context::Context < artis::utils::DoubleTime > SamaraContext;


#endif // DEFINES_HPP
