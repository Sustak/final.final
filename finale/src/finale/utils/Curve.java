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

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Vector;

/**
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class Curve implements ValueListener {

    private static HashMap<String,Curve> curves = new HashMap<String, Curve>();
    private String name;

    private Vector<ValuePair> values;
    private Vector<Estimator> linearEstimators;
    private Vector<Estimator> cubicEstimators;
    private Vector<ValuePairEstimated> estimatesLinear;
    private Vector<ValuePairEstimated> estimatesCubic;

    private static final int INIT_POINTS = 32;

    public static Curve getCurve( String name, ValuePair[] values, boolean sortByX ) {
        Curve curve = curves.get(name);
        if( curve == null ) {
            curve = new Curve(name, values, sortByX);
            curves.put(name, curve);
        }
        return curve;
    }

    Curve( String name, ValuePair[] values, boolean sortByX ) {
        this.name = name;
        if( values==null )
            this.values = new Vector<ValuePair>(INIT_POINTS);
        else
            this.values = new Vector<ValuePair>(Arrays.asList(values));
        sort();
    }

    void sort() {
        Collections.sort(values);
    }

    public void valueUpdate(Value value) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    private void generateLinearEstimators() {

    }
    

}
