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
import static java.lang.Math.*;

/**
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class ValuePair implements Comparable<ValuePair> {

    Value X, Y;
    public ValuePair(Value X, Value Y) {
        if( X==null || Y==null )
            throw new RuntimeException( "Null values in ValuePair not supported.");
        this.X = X;
        this.Y = Y;
    }
    public ValuePair(double X, double Y) {
        this.X = new ValueDouble(X);
        this.Y = new ValueDouble(Y);
    }

    public Value getX() {
        return X;
    }

    public Value getY() {
        return Y;
    }

//    public int compareTo(Object o) {
//        double value;
//        if( o==null )
//            value = 0.0;
//        else {
//            if( o instanceof Double ) {
//                value = (Double) o;
//            } else if( o instanceof Integer ) {
//                value = (Integer) o;
//            } else if( o instanceof Value ) {
//                value = ((Value)o).toDouble();
//            } else if( o instanceof ValuePair ) {
//                value = ((ValuePair)o).getX().toDouble();
//            } else {
//                throw new RuntimeException("Compare failed (ValuePair vs. " + o.getClass().getName());
//            }
//        }
//
//        double diff = value - getX().toDouble();
//        if( abs(diff)<Math.PRECISION ) {
//            return 0;
//        }
//        if( diff<0 )
//            return -1;
//
//        return 1;
//    }

    public int compareTo(ValuePair o) {

        double value;
        if( o==null )
            value = 0.0;
        else {
            value = o.getX().toDouble();
        }

        double diff = value - getX().toDouble();
        if( abs(diff)<Math.PRECISION ) {
            return 0;
        }
        if( diff<0 )
            return -1;

        return 1;
    }

}
