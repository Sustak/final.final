/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package finale;

import finale.utils.Date;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

/**
 *
 * @author Šesták
 */
public class CalendarTest {

    public CalendarTest() {
    }

	@BeforeClass
	public static void setUpClass() throws Exception {
	}

	@AfterClass
	public static void tearDownClass() throws Exception {
	}

	/**
	 * Test of isHoliday method, of class Calendar.
	 */
	@Test
	public void testIsHoliday() {
	}

	/**
	 * Test of getNextWorkDay method, of class Calendar.
	 */
	@Test
	public void testGetNextWorkDay() {
	}

	/**
	 * Test of getPrevWorkDay method, of class Calendar.
	 */
	@Test
	public void testGetPrevWorkDay() {
	}

	/**
	 * Test of getWorkDay method, of class Calendar.
	 */
	@Test
	public void testGetWorkDay() {
		Calendar cal = Calendar.getCalendar("ITALY-DEFAULT");
		Date date = Date.getValueOf(29, 6, 2009);
		Date endDate = cal.getWorkDay(date, 52*5+1);
		assertEquals( endDate, Date.getValueOf(16,7,2010) );
		Date begDate = cal.getWorkDay(endDate, -52*5-1 );
		assertEquals( date, begDate );
	}

	/**
	 * Test of isWeekDay method, of class Calendar.
	 */
	@Test
	public void testIsWeekDay() {
	}

	/**
	 * Test of getCalendar method, of class Calendar.
	 */
	@Test
	public void testGetCalendar() {
	}

	Date[] easterSundays = new Date[] {
		Date.getValueOf(23, 4, 2000),
		Date.getValueOf(15, 4, 2001),
		Date.getValueOf(31, 3, 2002),
		Date.getValueOf(20, 4, 2003),
		Date.getValueOf(11, 4, 2004),
		Date.getValueOf(27, 3, 2005),
		Date.getValueOf(16, 4, 2006),
		Date.getValueOf(8, 4, 2007),
		Date.getValueOf(23, 3, 2008),
		Date.getValueOf(12, 4, 2009),
		Date.getValueOf(4, 4, 2010),
		Date.getValueOf(24, 4, 2011),
		Date.getValueOf(8, 4, 2012),
		Date.getValueOf(31, 3, 2013),
		Date.getValueOf(20, 4, 2014),
		Date.getValueOf(5, 4, 2015),
		Date.getValueOf(27, 3, 2016),
		Date.getValueOf(16, 4, 2017),
		Date.getValueOf(1, 4, 2018),
		Date.getValueOf(21, 4, 2019),
		Date.getValueOf(12, 4, 2020)
	};
	/**
	 * Test of getEasterSunday method, of class Calendar.
	 */
	@Test
	public void testGetEasterSunday() {
		int year = 2000;
		for( Date easterSunday: easterSundays ) {
			assertEquals( easterSunday, Calendar.getEasterSunday(easterSunday.getYear()));
			year++;
		}
	}

	/**
	 * Test of getEasterFriday method, of class Calendar.
	 */
	@Test
	public void testGetEasterFriday() {
	}

	/**
	 * Test of getEasterMonday method, of class Calendar.
	 */
	@Test
	public void testGetEasterMonday() {
	}

}