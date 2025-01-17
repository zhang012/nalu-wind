// Copyright 2017 National Technology & Engineering Solutions of Sandia, LLC
// (NTESS), National Renewable Energy Laboratory, University of Texas Austin,
// Northwest Research Associates. Under the terms of Contract DE-NA0003525
// with NTESS, the U.S. Government retains certain rights in this software.
//
// This software is released under the BSD 3-clause license. See LICENSE file
// for more details.
//

#ifndef ProjectedNodalGradientEquationSystem_h
#define ProjectedNodalGradientEquationSystem_h

#include <Enums.h>
#include <EquationSystem.h>
#include <FieldTypeDef.h>
#include <NaluParsedTypes.h>

#include <stk_mesh/base/FieldBase.hpp>
#include <stk_mesh/base/CoordinateSystems.hpp>

namespace stk {
struct topology;
}

namespace sierra {
namespace nalu {

class Realm;
class AssembleNodalGradAlgorithmDriver;
class AlgorithmDriver;
class EquationSystems;

class ProjectedNodalGradientEquationSystem : public EquationSystem
{

public:
  ProjectedNodalGradientEquationSystem(
    EquationSystems& equationSystems,
    const EquationType eqType,
    const std::string dofName,
    const std::string deltaName,
    const std::string independentDofName,
    const std::string eqSysName,
    const bool managesSolve = false);
  virtual ~ProjectedNodalGradientEquationSystem();

  void set_data_map(BoundaryConditionType BC, std::string name);

  std::string get_name_given_bc(BoundaryConditionType BC);

  virtual void register_nodal_fields(const stk::mesh::PartVector& part_vec);

  void register_interior_algorithm(stk::mesh::Part* part);

  void register_wall_bc(
    stk::mesh::Part* part,
    const stk::topology& theTopo,
    const WallBoundaryConditionData& wallBCData);

  void register_inflow_bc(
    stk::mesh::Part* part,
    const stk::topology& theTopo,
    const InflowBoundaryConditionData& inflowBCData);

  void register_open_bc(
    stk::mesh::Part* part,
    const stk::topology& theTopo,
    const OpenBoundaryConditionData& openBCData);

  void register_symmetry_bc(
    stk::mesh::Part* part,
    const stk::topology& theTopo,
    const SymmetryBoundaryConditionData& symmetryBCData);

  // not supported
  void register_non_conformal_bc(
    stk::mesh::Part* part, const stk::topology& theTopo);

  void register_overset_bc();

  // internal solve and update from EquationSystems
  void solve_and_update();

  // external intended to be called by another EqSystem (used when someone
  // manages PNGEqs)
  void solve_and_update_external();

  void initialize();
  void reinitialize_linear_system();

  void deactivate_output();

  // names that customize this PNG system
  EquationType eqType_;
  std::string dofName_;
  std::string deltaName_;
  std::string independentDofName_;
  std::string eqSysName_;

  // who manages the solve? Often times, this is created by another EqSys
  const bool managesSolve_;

  // for exach equation, boundary data may be different
  std::map<BoundaryConditionType, std::string> dataMap_;

  // internal fields
  VectorFieldType* dqdx_;
  VectorFieldType* qTmp_;
};

} // namespace nalu
} // namespace sierra

#endif
