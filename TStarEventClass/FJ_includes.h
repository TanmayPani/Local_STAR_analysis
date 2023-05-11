R__ADD_INCLUDE_PATH(/opt/homebrew/Cellar/fastjet/3.4.0_1/include)
R__LOAD_LIBRARY(/opt/homebrew/Cellar/fastjet/3.4.0_1/lib/libfastjet.dylib)

#ifndef FJ_includes_H
#define FJ_includes_H

#include "fastjet/config.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/SISConePlugin.hh"
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/Selector.hh"
#include "fastjet/FunctionOfPseudoJet.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/BackgroundEstimatorBase.hh"
#include "fastjet/tools/Subtractor.hh"
//from contrib package
#include "fastjet/contrib/GenericSubtractor.hh"
#include "fastjet/contrib/ShapeWithComponents.hh"
#include "fastjet/contrib/ConstituentSubtractor.hh"
#include "fastjet/contrib/Nsubjettiness.hh"
#include "fastjet/contrib/Njettiness.hh"
#include "fastjet/contrib/NjettinessPlugin.hh"
#include "fastjet/contrib/SoftDrop.hh"

#endif
