/*
 * Finale, version 1.0
 *
 *  Copyright (C) 2010 Marek Šesták <marek.sestak@gmail.com>
 * 
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

package finale.utils;

/**
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class EstimatorPolynomial implements Estimator {

    private double[] coefficients;
    private int order;

    public EstimatorPolynomial(double a0, double a1, double a2, double a3) {
        coefficients = new double[]{a0,a1,a2,a3};
        order = 3;
    }

    public EstimatorPolynomial(double[] coefficients) {

        if( coefficients==null)
            order = -1;
        else
            order = coefficients.length - 1;

        if( order<0 )
            throw new RuntimeException("Polynomial Estimator were not provided with any coefficients." );

        this.coefficients = coefficients;
    }

    public double getEstimate(double x) {
        double result=0.0;
        for( int i=order; i>=0; i-- ) {
            result *= x;
            result += coefficients[i];
        }
        return result;
    }
}
