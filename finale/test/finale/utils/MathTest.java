/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package finale.utils;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Šesták
 */
public class MathTest {

    public MathTest() {
    }

	@BeforeClass
	public static void setUpClass() throws Exception {
	}

	@AfterClass
	public static void tearDownClass() throws Exception {
	}

	/**
	 * Test of Round method, of class Math.
	 */
	@Test
	public void testRound() {
		assertEquals( 1.0, Math.Round(1.0,0), 0.0000001 );
		assertEquals( 4.0, Math.Round(3.5,0), 0.0000001 );
		assertEquals( 3.1, Math.Round(3.14, 1), 0.0000001 );
		assertEquals( -2.0, Math.Round(-1.5, 0), 0.0000001 );
		assertEquals( 3.1, Math.Round(3.149,1), 0.0000001 );
		assertEquals( 1000.0, Math.Round(1234.0, -3), 0.0000001 );
	}
	/**
	 * Test of RoundDown method, of class Math.
	 */
	@Test
	public void testRoundDown() {
		assertEquals( 1.0, Math.RoundDown(1.0,0), 0.0000001 );
		assertEquals( 3.0, Math.RoundDown(3.5,0), 0.0000001 );
		assertEquals( 3.1, Math.RoundDown(3.14, 1), 0.0000001 );
		assertEquals( -1.0, Math.RoundDown(-1.5, 0), 0.0000001 );
		assertEquals( 3.14, Math.RoundDown(3.149,2), 0.0000001 );
		assertEquals( 9000.0, Math.RoundDown(9876.0, -3), 0.0000001 );
	}
	/**
	 * Test of RoundUp method, of class Math.
	 */
	@Test
	public void testRoundUp() {
		assertEquals( 1.0, Math.RoundUp(1.0,0), 0.0000001 );
		assertEquals( 4.0, Math.RoundUp(3.5,0), 0.0000001 );
		assertEquals( 3.2, Math.RoundUp(3.14, 1), 0.0000001 );
		assertEquals( -2.0, Math.RoundUp(-1.5, 0), 0.0000001 );
		assertEquals( 3.15, Math.RoundUp(3.149,2), 0.0000001 );
		assertEquals( 10000.0, Math.RoundUp(9876.0, -3), 0.0000001 );
	}

}