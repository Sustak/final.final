/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package finale;

import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author u03061
 */
public class PaymentFrequencyTest {

    public PaymentFrequencyTest() {
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
     * Test of toInt method, of class PaymentFrequency.
     */
    @Test
    public void testToInt() {
        System.out.println("toInt");

        PaymentFrequency instance = PaymentFrequency.IRREGULAR;
        assertEquals(0, instance.toInt());

        instance = PaymentFrequency.ANNUAL;
        assertEquals(1, instance.toInt());

        instance = PaymentFrequency.SEMIANNUAL;
        assertEquals(2, instance.toInt());

        instance = PaymentFrequency.TRIANNUAL;
        assertEquals(3, instance.toInt());

        instance = PaymentFrequency.QUATERLY;
        assertEquals(4, instance.toInt());

        instance = PaymentFrequency.BIMONTHLY;
        assertEquals(6, instance.toInt());

        instance = PaymentFrequency.MONTHLY;
        assertEquals(12, instance.toInt());

        instance = PaymentFrequency.WEEKLY;
        assertEquals(52, instance.toInt());

        instance = PaymentFrequency.DAILY;
        assertEquals(365, instance.toInt());

    }

    /**
     * Test of toPaymentFrequency method, of class PaymentFrequency.
     */
    @Test
    public void testToPaymentFrequency() {

        assertTrue(PaymentFrequency.IRREGULAR==PaymentFrequency.toPaymentFrequency(0));
        assertTrue(PaymentFrequency.ANNUAL==PaymentFrequency.toPaymentFrequency(1));
        assertTrue(PaymentFrequency.SEMIANNUAL==PaymentFrequency.toPaymentFrequency(2));
        assertTrue(PaymentFrequency.TRIANNUAL==PaymentFrequency.toPaymentFrequency(3));
        assertTrue(PaymentFrequency.QUATERLY==PaymentFrequency.toPaymentFrequency(4));
        assertTrue(PaymentFrequency.BIMONTHLY==PaymentFrequency.toPaymentFrequency(6));
        assertTrue(PaymentFrequency.MONTHLY==PaymentFrequency.toPaymentFrequency(12));
        assertTrue(PaymentFrequency.WEEKLY==PaymentFrequency.toPaymentFrequency(52));
        assertTrue(PaymentFrequency.DAILY==PaymentFrequency.toPaymentFrequency(365));

        for( int i=-1000; i<1000; i++ ) {
            if( i<0 || (i>4 && i!=6 && i!=12 && i!=52 && i!=365)) {
                
                try {
                    PaymentFrequency fq = PaymentFrequency.toPaymentFrequency(i);
                    fail("Code " + i + " failed to raise exception");
                }
                catch (Exception ex) {                
                }
                
            }
        }
    }

}