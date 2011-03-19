#include "weakform/weakform.h"
#include "integrals/integrals_h1.h"
#include "boundaryconditions/boundaryconditions.h"

// Exact solution.
class MyExactSolutionU : public ExactSolutionScalar
{
public:
  MyExactSolutionU(Mesh* mesh, double E, double nu, double lambda, double Q) 
            : ExactSolutionScalar(mesh), E(E), nu(nu), lambda(lambda), Q(Q) {
    k = 3.0 - 4.0 * nu;
    mu = E / (2.0 * (1.0 + nu));
    A = -E * (1 - nu * nu)/(1 - 2 * nu);
    B = -E * (1 - nu * nu)/(2 - 2 * nu);
    C = -E * (1 - nu * nu)/((1 - 2 * nu) * (2 - 2 * nu));
    D = 1.0 / (2 * mu);

    u_F = (k - Q * (lambda + 1));
    v_F = (k + Q * (lambda + 1));
  };
  
  double get_angle(double y, double x) {
    double theta = atan2(y, x);
    if (theta < 0)
      theta += 2 * M_PI;
    return theta;
  }

  double d_theta_dx(double x, double y) {
    return -y/(x*x + y*y);
  }

  double d_theta_dxd_theta_dx(double x, double y) {
    return 2*x*y/((x*x + y*y)*(x*x + y*y));
  }

  double d_theta_dy(double x, double y) {
    return x/(x*x + y*y) ;
  }

  double d_theta_dyd_theta_dy(double x, double y) {
    return -2*x*y/((x*x + y*y)*(x*x + y*y));
  }

  double d_theta_dxd_theta_dy(double x, double y) {
    return (y*y - x*x)/((x*x + y*y)*(x*x + y*y));
  }

  double r(double x, double y) {
    return pow((x*x + y*y), (lambda/2.0));  // r^labbda
  }

  double drdx(double x, double y) {
    return lambda * x * pow((x*x + y*y), (lambda/2.0 - 1.0));
  }

  double drdxdrdx(double x, double y) {
    return lambda * (pow((x*x + y*y), (lambda/2.0 - 1.0)) + (lambda - 2.0) 
           * x * x * pow((x*x + y*y), (lambda/2.0 - 2.0)));
  }

  double drdy(double x, double y) {
    return lambda * y * pow((x*x + y*y), (lambda/2.0 - 1.0));
  }

  double drdydrdy(double x, double y) {
    return lambda * (pow((x*x + y*y), (lambda/2.0 - 1.0)) + (lambda - 2.0) 
           * y * y * pow((x*x + y*y), (lambda/2.0 - 2.0)));
  }

  double drdxdrdy(double x, double y) {
    return lambda * 2.0 * x * y * (lambda/2.0 - 1) * pow((x*x + y*y), 
           (lambda/2.0 - 2.0));
  }

  double u_r(double x, double y) {
    return (u_F * cos(lambda * get_angle(y, x)) - lambda * cos((lambda - 2) 
           * get_angle(y, x)));
  }

  double du_rdx(double x, double y) {
    return (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dx(x, y)) 
           - (lambda * (-1) * (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));
  }

  double du_rdxdu_rdx(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dx(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) 
           * d_theta_dxd_theta_dx(x, y) + (lambda - 2) * d_theta_dx(x, y) * d_theta_dx(x, y) 
           * cos((lambda-2) * get_angle(y, x)) ));
  }

  double du_rdy(double x, double y) {
    return (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dy(x, y)) 
           - (lambda * (-1) * (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));
  }

  double du_rdydu_rdy(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + lambda * d_theta_dy(x, y) * d_theta_dy(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) 
           * d_theta_dyd_theta_dy(x, y) + (lambda - 2) * d_theta_dy(x, y) * d_theta_dy(x, y) 
           * cos((lambda-2) * get_angle(y, x)) ));
  }

  double du_rdxdu_rdy(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dy(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) 
           * d_theta_dxd_theta_dy(x, y) + (lambda - 2) * d_theta_dx(x, y) * d_theta_dy(x, y) 
           * cos((lambda-2) * get_angle(y, x)) ));
  }

  double v_r(double x, double y) {
    return (v_F * sin(lambda * get_angle(y, x)) + lambda * sin((lambda - 2) * get_angle(y, x)));
  }

  double dv_rdx(double x, double y) {
    return (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dx(x, y)) 
           + (lambda * (lambda - 2) * cos((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));
  }

  double dv_rdxdv_rdx(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dx(x, y) * (-1) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + (lambda - 2) * d_theta_dx(x, y) * d_theta_dx(x, y) * (-1) * sin((lambda-2) * get_angle(y, x)) ));
  }

  double dv_rdy(double x, double y) {
    return (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dy(x, y)) + (lambda * (lambda - 2) 
           * cos((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));
  }

  double dv_rdydv_rdy(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + lambda * d_theta_dy(x, y) * d_theta_dy(x, y) * (-1) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + (lambda - 2) * d_theta_dy(x, y) * d_theta_dy(x, y) * (-1) * sin((lambda-2) * get_angle(y, x)) ));
  }

  double dv_rdxdv_rdy(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + lambda * (-1) * d_theta_dx(x, y) * d_theta_dy(x, y) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + (lambda - 2) * (-1) * d_theta_dx(x, y) * d_theta_dy(x, y) * sin((lambda-2) * get_angle(y, x)) ));
  }

  // \partial^2 u / \partial x^2 
  double dudxdudx(double x, double y) {
    return D * (drdxdrdx(x, y) * u_r(x, y) + 2 * drdx(x, y) * du_rdx(x, y) + r(x, y) * du_rdxdu_rdx(x, y) );
  }

  double dudydudy(double x, double y) {
    return D * (drdydrdy(x, y) * u_r(x, y) + 2 * drdy(x, y) * du_rdy(x, y) + r(x, y) * du_rdydu_rdy(x, y) );
  }

  double dudxdudy(double x, double y) {
    return D * (drdxdrdy(x, y) * u_r(x, y) + drdx(x, y) * du_rdy(x, y) + drdy(x, y) * du_rdx(x, y) + r(x, y) 
           * du_rdxdu_rdy(x, y) );
  }

  // \partial^2 v / \partial x^2
  double dvdxdvdx(double x, double y) {
    return D * (drdxdrdx(x, y) * v_r(x, y) + 2 * drdx(x, y) * dv_rdx(x, y) + r(x, y) * dv_rdxdv_rdx(x, y) );
  }

  double dvdydvdy(double x, double y) {
    return D * (drdydrdy(x, y) * v_r(x, y) + 2 * drdy(x, y) * dv_rdy(x, y) + r(x, y) * dv_rdydv_rdy(x, y) );
  }

  double dvdxdvdy(double x, double y) {
    return D * (drdxdrdy(x, y) * v_r(x, y) + drdx(x, y) * dv_rdy(x, y) + drdy(x, y) * dv_rdx(x, y) + r(x, y) 
           * dv_rdxdv_rdy(x, y) );
  }

  // Exact solution u(x,y) and its derivatives.
  double u_fn(double x, double y) {
    return D * r(x, y) * u_r(x, y);
  }

  virtual scalar exact_function (double x, double y, scalar& dx, scalar& dy) {
    dx = D * drdx(x, y) * (u_F * cos(lambda * get_angle(y, x)) - lambda * cos((lambda - 2) * get_angle(y, x))) +
         D * r(x, y) * (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dx(x, y)) - 
         D * r(x, y) * (lambda * (-1) * (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));

    dy = D * drdy(x, y) * (u_F * cos(lambda * get_angle(y, x)) - lambda * cos((lambda - 2) * get_angle(y, x))) +
         D * r(x, y) * (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dy(x, y)) - 
         D * r(x, y) * (lambda * (-1) * (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));

    return u_fn(x, y);
  }

  // Members.
  double E, nu, lambda, Q, k, mu;
  double A, B, C, D, u_F, v_F;
};

class MyExactSolutionV : public ExactSolutionScalar
{
public:
  MyExactSolutionV(Mesh* mesh, double E, double nu, double lambda, double Q) 
            : ExactSolutionScalar(mesh), E(E), nu(nu), lambda(lambda), Q(Q) {
    k = 3.0 - 4.0 * nu;
    mu = E / (2.0 * (1.0 + nu));
    A = -E * (1 - nu * nu)/(1 - 2 * nu);
    B = -E * (1 - nu * nu)/(2 - 2 * nu);
    C = -E * (1 - nu * nu)/((1 - 2 * nu) * (2 - 2 * nu));
    D = 1.0 / (2 * mu);

    u_F = (k - Q * (lambda + 1));
    v_F = (k + Q * (lambda + 1));
  };
  
  double get_angle(double y, double x) {
    double theta = atan2(y, x);
    if (theta < 0)
      theta += 2 * M_PI;
    return theta;
  }

  double d_theta_dx(double x, double y) {
    return -y/(x*x + y*y);
  }

  double d_theta_dxd_theta_dx(double x, double y) {
    return 2*x*y/((x*x + y*y)*(x*x + y*y));
  }

  double d_theta_dy(double x, double y) {
    return x/(x*x + y*y) ;
  }

  double d_theta_dyd_theta_dy(double x, double y) {
    return -2*x*y/((x*x + y*y)*(x*x + y*y));
  }

  double d_theta_dxd_theta_dy(double x, double y) {
    return (y*y - x*x)/((x*x + y*y)*(x*x + y*y));
  }

  double r(double x, double y) {
    return pow((x*x + y*y), (lambda/2.0));  // r^labbda
  }

  double drdx(double x, double y) {
    return lambda * x * pow((x*x + y*y), (lambda/2.0 - 1.0));
  }

  double drdxdrdx(double x, double y) {
    return lambda * (pow((x*x + y*y), (lambda/2.0 - 1.0)) + (lambda - 2.0) 
           * x * x * pow((x*x + y*y), (lambda/2.0 - 2.0)));
  }

  double drdy(double x, double y) {
    return lambda * y * pow((x*x + y*y), (lambda/2.0 - 1.0));
  }

  double drdydrdy(double x, double y) {
    return lambda * (pow((x*x + y*y), (lambda/2.0 - 1.0)) + (lambda - 2.0) 
           * y * y * pow((x*x + y*y), (lambda/2.0 - 2.0)));
  }

  double drdxdrdy(double x, double y) {
    return lambda * 2.0 * x * y * (lambda/2.0 - 1) * pow((x*x + y*y), (lambda/2.0 - 2.0));
  }

  double u_r(double x, double y) {
    return (u_F * cos(lambda * get_angle(y, x)) - lambda * cos((lambda - 2) * get_angle(y, x)));
  }

  double du_rdx(double x, double y) {
    return (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dx(x, y)) 
           - (lambda * (-1) * (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));
  }

  double du_rdxdu_rdx(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dx(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + (lambda - 2) * d_theta_dx(x, y) * d_theta_dx(x, y) * cos((lambda-2) * get_angle(y, x)) ));
  }

  double du_rdy(double x, double y) {
    return (u_F * (-1) * lambda * sin(lambda * get_angle(y, x)) * d_theta_dy(x, y)) - (lambda * (-1) * 
           (lambda - 2) * sin((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));
  }

  double du_rdydu_rdy(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + lambda * d_theta_dy(x, y) * d_theta_dy(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + (lambda - 2) * d_theta_dy(x, y) * d_theta_dy(x, y) * cos((lambda-2) * get_angle(y, x)) ));
  }

  double du_rdxdu_rdy(double x, double y) {
    return (u_F * (-1) * lambda * (sin(lambda * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dy(x, y) * cos(lambda * get_angle(y, x)))) 
	   - (lambda * (-1) * (lambda - 2) * (sin((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + (lambda - 2) * d_theta_dx(x, y) * d_theta_dy(x, y) * cos((lambda-2) * get_angle(y, x)) ));
  }

  double v_r(double x, double y) {
    return (v_F * sin(lambda * get_angle(y, x)) + lambda * sin((lambda - 2) * get_angle(y, x)));
  }

  double dv_rdx(double x, double y) {
    return (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dx(x, y)) + (lambda * (lambda - 2) 
           * cos((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));
  }

  double dv_rdxdv_rdx(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + lambda * d_theta_dx(x, y) * d_theta_dx(x, y) * (-1) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dx(x, y) 
           + (lambda - 2) * d_theta_dx(x, y) * d_theta_dx(x, y) * (-1) * sin((lambda-2) * get_angle(y, x)) ));
  }

  double dv_rdy(double x, double y) {
    return (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dy(x, y)) + (lambda * (lambda - 2) 
           * cos((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));
  }

  double dv_rdydv_rdy(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + lambda * d_theta_dy(x, y) * d_theta_dy(x, y) * (-1) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dyd_theta_dy(x, y) 
           + (lambda - 2) * d_theta_dy(x, y) * d_theta_dy(x, y) * (-1) * sin((lambda-2) * get_angle(y, x)) ));
  }

  double dv_rdxdv_rdy(double x, double y) {
    return (v_F * lambda * (cos(lambda * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + lambda * (-1) * d_theta_dx(x, y) * d_theta_dy(x, y) * sin(lambda * get_angle(y, x)))) 
	   + (lambda * (lambda - 2) * (cos((lambda - 2) * get_angle(y, x)) * d_theta_dxd_theta_dy(x, y) 
           + (lambda - 2) * (-1) * d_theta_dx(x, y) * d_theta_dy(x, y) * sin((lambda-2) * get_angle(y, x)) ));
  }

  // \partial^2 u / \partial x^2 
  double dudxdudx(double x, double y) {
    return D * (drdxdrdx(x, y) * u_r(x, y) + 2 * drdx(x, y) * du_rdx(x, y) + r(x, y) * du_rdxdu_rdx(x, y) );
  }

  double dudydudy(double x, double y) {
    return D * (drdydrdy(x, y) * u_r(x, y) + 2 * drdy(x, y) * du_rdy(x, y) + r(x, y) * du_rdydu_rdy(x, y) );
  }

  double dudxdudy(double x, double y) {
    return D * (drdxdrdy(x, y) * u_r(x, y) + drdx(x, y) * du_rdy(x, y) + drdy(x, y) * du_rdx(x, y) + r(x, y) 
           * du_rdxdu_rdy(x, y) );
  }

  // \partial^2 v / \partial x^2
  double dvdxdvdx(double x, double y) {
    return D * (drdxdrdx(x, y) * v_r(x, y) + 2 * drdx(x, y) * dv_rdx(x, y) + r(x, y) * dv_rdxdv_rdx(x, y) );
  }

  double dvdydvdy(double x, double y) {
    return D * (drdydrdy(x, y) * v_r(x, y) + 2 * drdy(x, y) * dv_rdy(x, y) + r(x, y) * dv_rdydv_rdy(x, y) );
  }

  double dvdxdvdy(double x, double y) {
    return D * (drdxdrdy(x, y) * v_r(x, y) + drdx(x, y) * dv_rdy(x, y) + drdy(x, y) * dv_rdx(x, y) + r(x, y) 
           * dv_rdxdv_rdy(x, y) );
  }

  // Exact solution v(x,y) and its derivatives.
  double v_fn(double x, double y) {
    return D * r(x, y) * v_r(x, y);
  }

  virtual scalar exact_function (double x, double y, scalar& dx, scalar& dy) {
    dx = D * drdx(x, y) * (v_F * sin(lambda * get_angle(y, x)) + lambda * sin((lambda - 2) * get_angle(y, x))) +
         D * r(x, y) * (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dx(x, y)) + 
         D * r(x, y) * (lambda * (lambda - 2) * cos((lambda - 2) * get_angle(y, x)) * d_theta_dx(x, y));

    dy = D * drdy(x, y) * (v_F * sin(lambda * get_angle(y, x)) + lambda * sin((lambda - 2) * get_angle(y, x))) +
         D * r(x, y) * (v_F * lambda * cos(lambda * get_angle(y, x)) * d_theta_dy(x, y)) + 
         D * r(x, y) * (lambda * (lambda - 2) * cos((lambda - 2) * get_angle(y, x)) * d_theta_dy(x, y));

    return v_fn(x, y);
  }

  // Members.
  double E, nu, lambda, Q, k, mu;
  double A, B, C, D, u_F, v_F;
};

/* WEAK FORMS */

class MyWeakFormLinearElasticity : public WeakForm
{
public:
  MyWeakFormLinearElasticity(double E, double nu, double mu, double lambda) : WeakForm(2)
  {
    add_matrix_form(new MyMatrixFormVolLinearElasticity_0_0(E, nu));
    add_matrix_form(new MyMatrixFormVolLinearElasticity_0_1(E, nu));
    add_matrix_form(new MyMatrixFormVolLinearElasticity_1_1(E, nu));

    //add_vector_form(new VectorFormVolLinearElasticity_0(exact_solution_u));
    //add_vector_form(new VectorFormVolLinearElasticity_1(exact_solution_v));
  }

private:
  class MyMatrixFormVolLinearElasticity_0_0 : public WeakForm::MatrixFormVol
  {
  public:
    MyMatrixFormVolLinearElasticity_0_0(double E, double nu) : WeakForm::MatrixFormVol(0, 0), E(E), nu(nu) {
      A = -E * (1 - nu * nu)/(1 - 2 * nu);
      B = -E * (1 - nu * nu)/(2 - 2 * nu);
      sym = HERMES_SYM;
    }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
    {
      Scalar result = 0;
      for (int i = 0; i < n; i++)
        result += wt[i] * (A * u->dx[i] * v->dx[i] + B * u->dy[i] * v->dy[i]);
      return result;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext)
    {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext)
    {
       return Ord(30);
    }

    // Members.
    double A, B, E, nu;
  };

  class MyMatrixFormVolLinearElasticity_0_1 : public WeakForm::MatrixFormVol
  {
  public:
    MyMatrixFormVolLinearElasticity_0_1(double E, double nu) : WeakForm::MatrixFormVol(0, 1), E(E), nu(nu) { 
      C = -E * (1 - nu * nu)/((1 - 2 * nu) * (2 - 2 * nu));
      sym = HERMES_SYM;
    }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
    {
      Scalar result = 0;
      for (int i = 0; i < n; i++)
        result += wt[i] * (C * u->dx[i] * v->dy[i]);
      return result;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext)
    {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext)
    {
       return Ord(30);
    }

    // Members.
    double C, E, nu;
  };

  class MyMatrixFormVolLinearElasticity_1_1 : public WeakForm::MatrixFormVol
  {
  public:
    MyMatrixFormVolLinearElasticity_1_1(double E, double nu) : WeakForm::MatrixFormVol(1, 1), E(E), nu(nu) { 
      A = -E * (1 - nu * nu)/(1 - 2 * nu);
      B = -E * (1 - nu * nu)/(2 - 2 * nu);
      sym = HERMES_SYM;
    }

    template<typename Real, typename Scalar>
    Scalar matrix_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *u, Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext)
    {
      Scalar result = 0;
      for (int i = 0; i < n; i++)
        result += wt[i] * (B * u->dx[i] * v->dx[i] + A * u->dy[i] * v->dy[i]);
      return result;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *u, Func<double> *v, Geom<double> *e, ExtData<scalar> *ext)
    {
      return matrix_form<scalar, scalar>(n, wt, u_ext, u, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext)
    {
       return Ord(30);
    }
  
    // Members.
    double A, B, E, nu;
  };

  /*
  class VectorFormVolLinearElasticity_0 : public WeakForm::VectorFormVol
  {
  public:
    VectorFormVolLinearElasticity_0(ExactSolutionU* exact_solution) : WeakForm::VectorFormVol(0), 
    exact_solution(exact_solution) { }

    template<typename Real, typename Scalar>
    Scalar vector_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      Scalar result = 0;
      for (int i = 0; i < n; i++)
        result += wt[i] * (rhs_0<Real>(e->x[i], e->y[i]) * v->val[i]);
      return result;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return vector_form<scalar, scalar>(n, wt, u_ext, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return Ord(30);
    }

    template<typename Real>
    Real rhs_0(Real x, Real y)
    {
      return exact_solution->A * exact_solution->dudxdudx(x, y) + exact_solution->B * exact_solution->dudydudy(x, y) + exact_solution->C * exact_solution->dvdxdvdy(x, y);
    }

    // Member.
    MyExactSolutionU* exact_solution;
  };
  */

  /*
  class VectorFormVolLinearElasticity_1 : public WeakForm::VectorFormVol
  {
  public:
    VectorFormVolLinearElasticity_1(MyExactSolutionV* exact_solution) : WeakForm::VectorFormVol(1), 
    exact_solution(exact_solution) { }

    template<typename Real, typename Scalar>
    Scalar vector_form(int n, double *wt, Func<Scalar> *u_ext[], Func<Real> *v, Geom<Real> *e, ExtData<Scalar> *ext) {
      Scalar result = 0;
      for (int i = 0; i < n; i++)
        result += wt[i] * (rhs_1<Real>(e->x[i], e->y[i]) * v->val[i]);
      return result;
    }

    scalar value(int n, double *wt, Func<scalar> *u_ext[], Func<double> *v, Geom<double> *e, ExtData<scalar> *ext) {
      return vector_form<scalar, scalar>(n, wt, u_ext, v, e, ext);
    }

    Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, Geom<Ord> *e, ExtData<Ord> *ext) {
      return Ord(30);
    }

    template<typename Real>
    Real rhs_1(Real x, Real y)
    {
      return exact_solution->B * exact_solution->dvdxdvdx(x, y) + exact_solution->A * exact_solution->dvdydvdy(x, y) + exact_solution->C * exact_solution->dudxdudy(x, y);
    }

    // Member.
    MyExactSolutionV* exact_solution;
  };
  */
};

class DirichletNonConstantExactU : public DirichletBoundaryCondition
{
public:
  DirichletNonConstantExactU(std::string marker, MyExactSolutionU* exact_solution) : 
        DirichletBoundaryCondition(Hermes::vector<std::string>()), exact_solution(exact_solution) 
  {
    markers.push_back(marker);
  }
  
  ~DirichletNonConstantExactU() {}

  virtual BoundaryConditionValueType get_value_type() const { 
    return BC_FUNCTION; 
  }

  virtual scalar function(double x, double y) const {
    return exact_solution->u_fn(x, y);
  }

  MyExactSolutionU* exact_solution;
};

class DirichletNonConstantExactV : public DirichletBoundaryCondition
{
public:
  DirichletNonConstantExactV(std::string marker, MyExactSolutionV* exact_solution) : 
        DirichletBoundaryCondition(Hermes::vector<std::string>()), exact_solution(exact_solution) 
  {
    markers.push_back(marker);
  }
  
  ~DirichletNonConstantExactV() {}

  virtual BoundaryConditionValueType get_value_type() const { 
    return BC_FUNCTION; 
  }

  virtual scalar function(double x, double y) const {
    return exact_solution->v_fn(x, y);
  }

  MyExactSolutionV* exact_solution;
};