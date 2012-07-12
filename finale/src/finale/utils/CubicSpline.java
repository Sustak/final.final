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

import Jama.Matrix;
import java.util.Arrays;
import java.util.Comparator;

/**
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class CubicSpline implements Estimator {

    private int nValues = 0;
    private double[][] values;
    private double[][] coefficients;

    /**
     * Public constructor used to create cubic spline.
     * @param values Two-dimensional rectangular array containing points used to construct
     * cubic spline. First dimension must have length of 2 (the items with indexes
     * larger than 1 are ignored), length of second dimension is equal to the number
     * of points. So for instance <code>values[0][0]</code> contains first point's X coordinate and
     * <code>values[1][0]</code> contains its Y coordinate. The array will be sorted
     * by the values contained in first row (that is by X coordinates).
     * @throws IllegalArgumentException if number of points is smaller than 4 or
     * if X coordinates contain duplicity.
     */
    public CubicSpline(double[][] values) {

        if (values != null) {
            nValues = values.length;
        }

        if (nValues < 2) {
            throw new IllegalArgumentException("Cubic spline cannot be constructed, no or too few points provided");
        }

        this.values = values;

        if (nValues == 2) {
            SortValues();
            coefficients = new double[4][1];
            coefficients[3][0] = values[0][1];
            coefficients[2][0] = (values[1][1] - values[0][1]) / (values[1][0] - values[0][0]);
        }
        CalculatePolynomials();
    }

    private void CalculatePolynomials() {

        SortValues();
        int i;

        double[] h = new double[nValues - 1];
        for (i = 0; i < (nValues - 1); i++) {
            h[i] = values[i + 1][0] - values[i][0];
        }

        double[][] A = new double[nValues - 2][nValues - 2];
        double[][] B = new double[1][nValues - 2];
        for (i = 0; i < (nValues - 2); i++) {
            A[i][i] = 2 * (h[i] + h[i + 1]);
            B[0][i] = 6 * (((values[i + 2][1] - values[i + 1][1]) / h[i + 1])
                    - ((values[i + 1][1] - values[i][1]) / h[i]));
        }
        for (i = 1; i < (nValues - 2); i++) {
            A[i][i - 1] = A[i - 1][i] = h[i];
        }

        Matrix mA = new Matrix(A);
        Matrix mB = new Matrix(B);
        Matrix mAInverse = mA.inverse();
        Matrix mS = mB.times(mAInverse);

        double[] S = new double[nValues];
        for (i = 1; i < nValues - 1; i++) {
            S[i] = mS.get(0, i - 1);
        }

        coefficients = new double[4][nValues - 1];
        for (i = 0; i < nValues - 1; i++) {
            coefficients[0][i] = (S[i + 1] - S[i]) / (6 * h[i]);
            coefficients[1][i] = S[i] / 2;
            coefficients[2][i] = ((values[i + 1][1] - values[i][1]) / h[i])
                    - (2 * h[i] * S[i] + h[i] * S[i + 1]) / 6;
            coefficients[3][i] = values[i][1];
        }
    }

    private void SortValues() {

        Arrays.sort(values, new ComparatorImpl());
    }

    public double getEstimate(double x) {

        int i;

        if (nValues == 2) {
            i = 0;
        } else {
            i = Arrays.binarySearch(values, new double[]{x, 0.0}, new ComparatorImpl());
            if (i < 0) {
                i = (-i) - 2;
                if (i < 0) {
                    i = 0;
                }
            }
            if (i > (nValues - 2)) {
                i = nValues - 2;
            }
        }

        x -= values[i][0];
        double x2 = x * x;
        double x3 = x2 * x;
        double estimate = coefficients[3][i] + coefficients[2][i] * x
                + coefficients[1][i] * x2 + coefficients[0][i] * x3;

        return estimate;

        // TODO: an option to return straight line estimate for points
        // before first and after last X coordinates.
    }

    private static class ComparatorImpl implements Comparator {

        public ComparatorImpl() {
        }

        public int compare(Object o1, Object o2) {
            double[] row1 = (double[]) o1;
            double[] row2 = (double[]) o2;
            if (row1[0] < row2[0]) {
                return -1;
            }
            if (row1[0] > row2[0]) {
                return 1;
            }
            return 0;
        }
    }
}
