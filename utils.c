#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils.h"

/* apply rules, given a function, then calculate the
 * result by the region in node by using Gauss-Kronrod Quadrature Formula, 
 * and update the result and error in node
 */

void rule(double (* f)(double), node_ptr node)
{
	/* weights for Gauss Quadrature */

	static double wg[4] = {
		0.129484966168869693270611432679082,
	    0.27970539148927666790146777142378,
	    0.381830050505118944950369775488975,
	    0.417959183673469387755102040816327 
	};
	
	/* weights for Kronrod Quadrature */

	static double wgk[8] = {
		0.02293532201052922496373200805897,
	    0.063092092629978553290700663189204,
	    0.104790010322250183839876322541518,
	    0.140653259715525918745189590510238,
	    0.16900472663926790282658342659855,
	    0.190350578064785409913256402421014,
	    0.204432940075298892414161999234649,
	    0.209482141084727828012999174891714
	};

	/* points selected in x axis */

	static double xgk[8] = { 
		0.991455371120812639206854697526329,
	    0.949107912342758524526189684047851,
	    0.864864423359769072789712788640926,
	    0.741531185599394439863864773280788,
	    0.58608723546769113029414483825873,
	    0.405845151377397166906606412076961,
	    0.207784955007898467600689403773245,
		0.0 
	};
    
	
	int j;
	double d__1, d__2, d__3;
	double fsum, fval1, fval2;
	double fc, fv1[7], fv2[7];
	double resg, resk, reskh, absc, abserr;
	double resabs, resasc;
	int jtw, jtwm1;
	double center, hlgth, dhlgth;
	
	/* variables used for region switch */
	center = (node->start + node->end) * 0.5;
	hlgth = (node->end - node->start) * 0.5;
	dhlgth = fabs(hlgth);
	
	fc = (* f)(center);
	resg = fc * wg[3];
	resk = fc *wgk[7];
	resabs = fabs(resk);
	
	/* fill odd positions in array */
	for (j = 1; j <= 3; j++) {
		jtw = j << 1;
		absc = hlgth * xgk[jtw - 1];
		d__1 = center - absc;
		fval1 = (* f)(d__1);
		d__1 = center + absc;
		fval2 = (* f)(d__1);
		fv1[jtw - 1] = fval1;
		fv2[jtw - 1] = fval2;
		fsum = fval1 + fval2;
		resg += wg[j - 1] * fsum;
		resk += wgk[jtw - 1] * fsum;
		resabs += wgk[jtw - 1] * (fabs(fval1) + fabs(fval2));
	}
	
	/* fill even positions in array */
	for (j = 1; j <= 4; j++) {
		jtwm1 = (j << 1) - 1;
		absc = hlgth * xgk[jtwm1 - 1];
		d__1 = center - absc;
		fval1 = (* f)(d__1);
		d__1 = center + absc;
		fval2 = (* f)(d__1);
		fv1[jtwm1 - 1] = fval1;
		fv2[jtwm1 - 1] = fval2;
		fsum = fval1 + fval2;
		resk += wgk[jtwm1 - 1] * fsum;
		resabs += wgk[jtwm1 - 1] * (fabs(fval1) + fabs(fval2));
	}
	
	reskh = resk * 0.5;
	resasc = wgk[7] * (d__1 = fc - reskh, fabs(d__1));
	for (j = 1; j <= 7; j++) {
		resasc += wgk[j - 1] * ((d__1 = fv1[j - 1] - reskh, fabs(d__1)) +
								(d__2 = fv2[j - 1] - reskh, fabs(d__2)));
	}
	
	/* use value from Kronrod Quadrature as the result */
	node->result = resk * hlgth;
	resabs *= dhlgth;
	resasc *= dhlgth;
	/* use Kronrod Quadrature minus Gauss Quadrature as the error */
	node->error = abserr = (d__1 = (resk - resg) * hlgth, fabs(d__1));

	if (resasc != 0.0 && abserr != 0)
	{
		d__3 = abserr * 200.0 / resasc;
		d__1 = 1.0;
		d__2 = pow(d__3, 1.5);
		node->error = abserr = resasc * min(d__1, d__2);
	}
	if (resabs > (1e-300 / (1e-18 * 50)))
	{
		d__1 = 1e-18 * 50 * resabs;
		node->error = abserr = max(d__1, abserr);
	}
}

