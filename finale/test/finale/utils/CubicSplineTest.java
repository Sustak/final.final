/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package finale.utils;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 */
public class CubicSplineTest {

    public CubicSplineTest() {
    }

    @BeforeClass
    public static void setUpClass() throws Exception {
    }

    @AfterClass
    public static void tearDownClass() throws Exception {
    }

    @Before
    public void setUp() {
    }

    @After
    public void tearDown() {
    }

    /**
     * Test of getEstimate method, of class CubicSpline.
     */
    @Test
    public void testGetEstimate() {

        System.out.println("getEstimate");

        double[][] vals = new double[][]{
            {13, 10},
            {12, 15},
            {6, 14},
            {5, 11},
            {2, 8},
            {1, 5}
        };
        CubicSpline instance = new CubicSpline(vals);

        assertEquals(5.718, instance.getEstimate(-2), 0.001);
        assertEquals(4.058, instance.getEstimate(-1.75), 0.001);
        assertEquals(2.815, instance.getEstimate(-1.5), 0.001);
        assertEquals(1.951, instance.getEstimate(-1.25), 0.001);
        assertEquals(1.43, instance.getEstimate(-1), 0.001);
        assertEquals(1.212, instance.getEstimate(-0.75), 0.001);
        assertEquals(1.259, instance.getEstimate(-0.5), 0.001);
        assertEquals(1.535, instance.getEstimate(-0.25), 0.001);
        assertEquals(2, instance.getEstimate(0), 0.001);
        assertEquals(2.617, instance.getEstimate(0.25), 0.001);
        assertEquals(3.348, instance.getEstimate(0.5), 0.001);
        assertEquals(4.155, instance.getEstimate(0.75), 0.001);
        assertEquals(5, instance.getEstimate(1), 0.001);
        assertEquals(5.845, instance.getEstimate(1.25), 0.001);
        assertEquals(6.652, instance.getEstimate(1.5), 0.001);
        assertEquals(7.383, instance.getEstimate(1.75), 0.001);
        assertEquals(8, instance.getEstimate(2), 0.001);
        assertEquals(8.476, instance.getEstimate(2.25), 0.001);
        assertEquals(8.825, instance.getEstimate(2.5), 0.001);
        assertEquals(9.074, instance.getEstimate(2.75), 0.001);
        assertEquals(9.248, instance.getEstimate(3), 0.001);
        assertEquals(9.372, instance.getEstimate(3.25), 0.001);
        assertEquals(9.472, instance.getEstimate(3.5), 0.001);
        assertEquals(9.574, instance.getEstimate(3.75), 0.001);
        assertEquals(9.703, instance.getEstimate(4), 0.001);
        assertEquals(9.884, instance.getEstimate(4.25), 0.001);
        assertEquals(10.144, instance.getEstimate(4.5), 0.001);
        assertEquals(10.507, instance.getEstimate(4.75), 0.001);
        assertEquals(11, instance.getEstimate(5), 0.001);
        assertEquals(11.636, instance.getEstimate(5.25), 0.001);
        assertEquals(12.379, instance.getEstimate(5.5), 0.001);
        assertEquals(13.183, instance.getEstimate(5.75), 0.001);
        assertEquals(14, instance.getEstimate(6), 0.001);
        assertEquals(14.791, instance.getEstimate(6.25), 0.001);
        assertEquals(15.544, instance.getEstimate(6.5), 0.001);
        assertEquals(16.255, instance.getEstimate(6.75), 0.001);
        assertEquals(16.922, instance.getEstimate(7), 0.001);
        assertEquals(17.539, instance.getEstimate(7.25), 0.001);
        assertEquals(18.104, instance.getEstimate(7.5), 0.001);
        assertEquals(18.613, instance.getEstimate(7.75), 0.001);
        assertEquals(19.061, instance.getEstimate(8), 0.001);
        assertEquals(19.446, instance.getEstimate(8.25), 0.001);
        assertEquals(19.763, instance.getEstimate(8.5), 0.001);
        assertEquals(20.008, instance.getEstimate(8.75), 0.001);
        assertEquals(20.179, instance.getEstimate(9), 0.001);
        assertEquals(20.27, instance.getEstimate(9.25), 0.001);
        assertEquals(20.279, instance.getEstimate(9.5), 0.001);
        assertEquals(20.202, instance.getEstimate(9.75), 0.001);
        assertEquals(20.035, instance.getEstimate(10), 0.001);
        assertEquals(19.773, instance.getEstimate(10.25), 0.001);
        assertEquals(19.414, instance.getEstimate(10.5), 0.001);
        assertEquals(18.954, instance.getEstimate(10.75), 0.001);
        assertEquals(18.388, instance.getEstimate(11), 0.001);
        assertEquals(17.714, instance.getEstimate(11.25), 0.001);
        assertEquals(16.927, instance.getEstimate(11.5), 0.001);
        assertEquals(16.024, instance.getEstimate(11.75), 0.001);
        assertEquals(15, instance.getEstimate(12), 0.001);
        assertEquals(13.858, instance.getEstimate(12.25), 0.001);
        assertEquals(12.624, instance.getEstimate(12.5), 0.001);
        assertEquals(11.327, instance.getEstimate(12.75), 0.001);
        assertEquals(10, instance.getEstimate(13), 0.001);
        assertEquals(8.673, instance.getEstimate(13.25), 0.001);
        assertEquals(7.376, instance.getEstimate(13.5), 0.001);
        assertEquals(6.142, instance.getEstimate(13.75), 0.001);
        assertEquals(5, instance.getEstimate(14), 0.001);
        assertEquals(3.982, instance.getEstimate(14.25), 0.001);
        assertEquals(3.119, instance.getEstimate(14.5), 0.001);
        assertEquals(2.442, instance.getEstimate(14.75), 0.001);
        assertEquals(1.982, instance.getEstimate(15), 0.001);
        assertEquals(1.77, instance.getEstimate(15.25), 0.001);
        assertEquals(1.836, instance.getEstimate(15.5), 0.001);
        assertEquals(2.212, instance.getEstimate(15.75), 0.001);
        assertEquals(2.928, instance.getEstimate(16), 0.001);
        assertEquals(4.016, instance.getEstimate(16.25), 0.001);
        assertEquals(5.507, instance.getEstimate(16.5), 0.001);
        assertEquals(7.432, instance.getEstimate(16.75), 0.001);

    }

    @Test
    public void testGetEstimate2() {

        System.out.println("getEstimate2");

        double[][] vals = new double[][]{
            {13, 10},
            {2, 8},
            {1, 5}
        };
        CubicSpline instance = new CubicSpline(vals);
        assertEquals(-1.182, instance.getEstimate(-2), 0.001);
        assertEquals(-1.131, instance.getEstimate(-1.75), 0.001);
        assertEquals(-0.959, instance.getEstimate(-1.5), 0.001);
        assertEquals(-0.677, instance.getEstimate(-1.25), 0.001);
        assertEquals(-0.295, instance.getEstimate(-1), 0.001);
        assertEquals(0.174, instance.getEstimate(-0.75), 0.001);
        assertEquals(0.72, instance.getEstimate(-0.5), 0.001);
        assertEquals(1.333, instance.getEstimate(-0.25), 0.001);
        assertEquals(2, instance.getEstimate(0), 0.001);
        assertEquals(2.711, instance.getEstimate(0.25), 0.001);
        assertEquals(3.456, instance.getEstimate(0.5), 0.001);
        assertEquals(4.222, instance.getEstimate(0.75), 0.001);
        assertEquals(5, instance.getEstimate(1), 0.001);
        assertEquals(5.778, instance.getEstimate(1.25), 0.001);
        assertEquals(6.544, instance.getEstimate(1.5), 0.001);
        assertEquals(7.289, instance.getEstimate(1.75), 0.001);
        assertEquals(8, instance.getEstimate(2), 0.001);
        assertEquals(8.669, instance.getEstimate(2.25), 0.001);
        assertEquals(9.296, instance.getEstimate(2.5), 0.001);
        assertEquals(9.88, instance.getEstimate(2.75), 0.001);
        assertEquals(10.424, instance.getEstimate(3), 0.001);
        assertEquals(10.927, instance.getEstimate(3.25), 0.001);
        assertEquals(11.391, instance.getEstimate(3.5), 0.001);
        assertEquals(11.817, instance.getEstimate(3.75), 0.001);
        assertEquals(12.207, instance.getEstimate(4), 0.001);
        assertEquals(12.56, instance.getEstimate(4.25), 0.001);
        assertEquals(12.878, instance.getEstimate(4.5), 0.001);
        assertEquals(13.162, instance.getEstimate(4.75), 0.001);
        assertEquals(13.413, instance.getEstimate(5), 0.001);
        assertEquals(13.632, instance.getEstimate(5.25), 0.001);
        assertEquals(13.82, instance.getEstimate(5.5), 0.001);
        assertEquals(13.978, instance.getEstimate(5.75), 0.001);
        assertEquals(14.107, instance.getEstimate(6), 0.001);
        assertEquals(14.208, instance.getEstimate(6.25), 0.001);
        assertEquals(14.282, instance.getEstimate(6.5), 0.001);
        assertEquals(14.33, instance.getEstimate(6.75), 0.001);
        assertEquals(14.353, instance.getEstimate(7), 0.001);
        assertEquals(14.352, instance.getEstimate(7.25), 0.001);
        assertEquals(14.328, instance.getEstimate(7.5), 0.001);
        assertEquals(14.282, instance.getEstimate(7.75), 0.001);
        assertEquals(14.215, instance.getEstimate(8), 0.001);
        assertEquals(14.128, instance.getEstimate(8.25), 0.001);
        assertEquals(14.022, instance.getEstimate(8.5), 0.001);
        assertEquals(13.897, instance.getEstimate(8.75), 0.001);
        assertEquals(13.756, instance.getEstimate(9), 0.001);
        assertEquals(13.599, instance.getEstimate(9.25), 0.001);
        assertEquals(13.427, instance.getEstimate(9.5), 0.001);
        assertEquals(13.241, instance.getEstimate(9.75), 0.001);
        assertEquals(13.041, instance.getEstimate(10), 0.001);
        assertEquals(12.83, instance.getEstimate(10.25), 0.001);
        assertEquals(12.608, instance.getEstimate(10.5), 0.001);
        assertEquals(12.376, instance.getEstimate(10.75), 0.001);
        assertEquals(12.134, instance.getEstimate(11), 0.001);
        assertEquals(11.885, instance.getEstimate(11.25), 0.001);
        assertEquals(11.629, instance.getEstimate(11.5), 0.001);
        assertEquals(11.366, instance.getEstimate(11.75), 0.001);
        assertEquals(11.099, instance.getEstimate(12), 0.001);
        assertEquals(10.828, instance.getEstimate(12.25), 0.001);
        assertEquals(10.554, instance.getEstimate(12.5), 0.001);
        assertEquals(10.277, instance.getEstimate(12.75), 0.001);
        assertEquals(10, instance.getEstimate(13), 0.001);
        assertEquals(9.723, instance.getEstimate(13.25), 0.001);
        assertEquals(9.446, instance.getEstimate(13.5), 0.001);
        assertEquals(9.172, instance.getEstimate(13.75), 0.001);
        assertEquals(8.901, instance.getEstimate(14), 0.001);
        assertEquals(8.634, instance.getEstimate(14.25), 0.001);
        assertEquals(8.371, instance.getEstimate(14.5), 0.001);
        assertEquals(8.115, instance.getEstimate(14.75), 0.001);
        assertEquals(7.866, instance.getEstimate(15), 0.001);
        assertEquals(7.624, instance.getEstimate(15.25), 0.001);
        assertEquals(7.392, instance.getEstimate(15.5), 0.001);
        assertEquals(7.17, instance.getEstimate(15.75), 0.001);
        assertEquals(6.959, instance.getEstimate(16), 0.001);
        assertEquals(6.759, instance.getEstimate(16.25), 0.001);
        assertEquals(6.573, instance.getEstimate(16.5), 0.001);
        assertEquals(6.401, instance.getEstimate(16.75), 0.001);
    }
}
