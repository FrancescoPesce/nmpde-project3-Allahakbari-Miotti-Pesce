#include <deal.II/lac/solver_control.h>
#include <deal.II/lac/solver_gmres.h>
#include <deal.II/numerics/vector_tools.h>

#include "NavierStokes.hpp"
#include "Precondition.hpp"

template class NavierStokes<2U>;
template class NavierStokes<3U>;

template <unsigned int dim>
void NavierStokes<dim>::apply_initial_conditions() {
  pcout << "Applying the initial conditions" << std::endl;

  // Since only velocity has an initial condition, create a mask.
  ComponentMask mask;
  if constexpr (dim == 2) {
    mask = ComponentMask({true, true, false});
  } else {
    mask = ComponentMask({true, true, true, false});
  }

  VectorTools::interpolate(dof_handler, *initial_conditions, solution_owned,
                           mask);
  solution = solution_owned;
}

template <unsigned int dim>
void NavierStokes<dim>::solve_time_step() {
  pcout << "  Solving the linear system" << std::endl;

  SolverControl solver_control(10000, 1e-6 * system_rhs.l2_norm());

  SolverGMRES<TrilinosWrappers::MPI::BlockVector> solver(solver_control);

  pcout << "  Building the preconditioner" << std::endl;

  PreconditionaSIMPLE precondition;
  constexpr double alpha = 1.0;
  precondition.initialize(system_matrix.block(0, 0), system_matrix.block(1, 0),
                          system_matrix.block(0, 1), solution_owned, alpha);

  pcout << "  Solving the system" << std::endl;
  solver.solve(system_matrix, solution_owned, system_rhs, precondition);
  pcout << "  " << solver_control.last_step() << " GMRES iterations"
        << std::endl;

  solution = solution_owned;
}

template <unsigned int dim>
void NavierStokes<dim>::solve() {
  assemble_constant();

  pcout << "===============================================" << std::endl;

  // Calculate and output the initial solution.
  time_step = 0;
  apply_initial_conditions();
  output();

  time_step = 1;
  double time = deltat;

  // Solve the problem at each time step.
  while (time < T + deltat) {
    pcout << "n = " << std::setw(3) << time_step << ", t = " << std::setw(5)
          << time << ": " << std::endl;

    assemble_time_dependent();
    solve_time_step();
    output();

    time += deltat;
    ++time_step;
  }
}
