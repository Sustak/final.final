/*
 *  Finale, Copyright (c) 2008 Marek Šesták
 *  This programm has been released under GNU GPL v2.0
 */
package finale.utils;

import java.util.Calendar;

/**
 *
 * @author Marek Šesták, marek.sestak@gmail.com
 */
public class Date
		implements Comparable<Date> {

	private static Date[] dateCache;    // cache of all dates that are created just ones
	private static int cacheBaseIndex;  // serial number of the first date in the cache
	private static int cacheSize;       // number of dates in the cache
	private static Calendar calendar;   // default calendar
	private static int today;           // serial number of today
	static final int DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static final int DAYS_IN_MONTH_CUM[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};


	static {
		calendar = Calendar.getInstance();
		int currentYear = calendar.get(Calendar.YEAR);
		int currentMonth = calendar.get(Calendar.MONTH) + 1;
		// January is returned as 0, hence the returned value is increased by 1
		int currentDay = calendar.get(Calendar.DAY_OF_MONTH);

		today = Date.getSerial(currentDay, currentMonth, currentYear);

		cacheBaseIndex = Date.getSerial(1, 1, currentYear - 30);
		int endcache = Date.getSerial(1, 1, currentYear + 50);
		cacheSize = endcache - cacheBaseIndex + 1;

		dateCache = new Date[cacheSize];
	}

	public static Date getValueOf(String value) {

		String[] date = value.split("\\.");
		if (date.length != 3) {
			throw new IllegalArgumentException("String " + value +
					" cannot be converted to a date");
		}
		int day = Integer.valueOf(date[0]);
		int month = Integer.valueOf(date[1]);
		int year = Integer.valueOf(date[2]);
		if (year < 100) {
			if (year < 70) {
				year += 2000;
			} else {
				year += 1900;
			}
		}

		return Date.getValueOf(day, month, year);
	}

	public int getDay() {
		return day;
	}
	private int day;

	public int getMonth() {
		return month;
	}
	private int month;

	public int getYear() {
		return year;
	}
	private int year;

	public int getSerial() {
		return serial;
	}
	private int serial;

	public boolean isLeap() {
		return isLeap;
	}
	private boolean isLeap;

	public boolean isMonthEnd() {
		return isMonthEnd;
	}
	private boolean isMonthEnd;

	protected Date(int day, int month, int year) {
		this.day = day;
		this.month = month;
		this.year = year;

		if (Date.isValid(day, month, year) == false) {
			throw new IllegalArgumentException("Date (day:" + day + ", month:" + month +
					", year:" + year + ") isn't valid and cannot be constructed.");
		}

		this.serial = Date.getSerial(day, month, year);
		this.isLeap = Date.isLeap(year);
		this.isMonthEnd = Date.isMonthEnd(day, month, year);
	}

	protected Date(int serial) {

		if (serial <= 0) {
			throw new IllegalArgumentException("Date with a serial number " +
					serial + " isn't valid and cannot be constructed.");
		}
		this.serial = serial;

		if (serial < 367) {
			year = 1900;
			this.isLeap = true;
		} else {
			year = 1900 + (serial - 2 - ((serial - 365) / 1460) + (serial - 368) / 36524 - (serial + 109205) / (146096)) / 365;
			this.isLeap = Date.isLeap(this.year);
		}

		int january1Serial = Date.getSerial(1, 1, this.year);

		serial = serial - january1Serial + 1;

		for (month = 1;
				month < 13 && serial > ((month == 2 && this.isLeap ? 1 : 0) + DAYS_IN_MONTH[month - 1]);
				month++) {
			serial = serial - DAYS_IN_MONTH[month - 1] - (month == 2 && this.isLeap ? 1 : 0);
		}
		day = serial;

		if (Date.isValid(day, month, year) == false) {
			throw new IllegalArgumentException("Date with a serial number " +
					serial + " isn't valid and cannot be constructed.");
		}

		this.isLeap = Date.isLeap(year);
		this.isMonthEnd = Date.isMonthEnd(day, month, year);
	}

	@Override
	public String toString() {
		return String.format("%02d.%02d.%04d", day, month, year);
	}

	public static Date getInstance() {
		return Date.getValueOf(today);
	}

	@Override
	public boolean equals(Object o) {
		if (!(o instanceof Date)) {
			return false;
		}

		return ((Date) o).getSerial() == this.getSerial();
	}

	public int compareTo(Date date) {
		if (this.serial < date.serial) {
			return -1;
		}
		if (this.serial > date.serial) {
			return 1;
		}
		return 0;
	}

	public static int compare(Date date1, Date date2) {
		if (date1.serial < date2.serial) {
			return -1;
		}
		if (date1.serial > date2.serial) {
			return 1;
		}
		return 0;
	}

	public int diff(Date date) {
		return date.serial - this.serial;
	}

	public static int diff(Date date1, Date date2) {
		return date2.serial - date1.serial;
	}

	public int toInt() {
		return serial;
	}

	@Override
	public int hashCode() {
		int hash = 7;
		hash = 59 * hash + this.serial;
		return hash;
	}

	public static Date getValueOf(int serial) {

		if (serial <= 0) {
			throw new IllegalArgumentException("Date with a serial number " +
					serial + " isn't valid and cannot be constructed.");
		}

		int index = serial - Date.cacheBaseIndex;
		if (index < 0 || index >= Date.cacheSize) {
			return new Date(serial);
		}

		if (dateCache[index] == null) {
			dateCache[index] = new Date(serial);
		}

		return dateCache[index];
	}

	public static Date getValueOf(int day, int month, int year) {

		if (!Date.isValid(day, month, year)) {
			throw new IllegalArgumentException("Date doesn't exist ( day:" +
					day + ",month:" + month + ", year " + year + " )");
		}

		int aserial = Date.getSerial(day, month, year);

		// dates outside of the cache will be constructed in
		//   a traditional way and returned to the caller
		int index = aserial - cacheBaseIndex;
		if (aserial < cacheBaseIndex || index >= cacheSize) {
			return new Date(day, month, year);
		} else {
			if (dateCache[index] == null) {
				dateCache[index] = new Date(day, month, year);
			}
			return dateCache[index];
		}
	}

	/**
	 * This is IsLeap function
	 * @param year
	 * @return for leap years true is returned, otherwise false is returned
	 */
	public static boolean isLeap(int year) {
		if (year % 4 == 0) {
			if (year % 100 == 0) {
				if (year % 400 == 0) {
					return true;
				} else {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	public static boolean isMonthEnd(int day, int month, int year) {
		return day == Date.lastDayOfMonth(month, year);
	}

	/**
	 * Function IsValid checks, whether given combination of a day/month/year
	 * represents a valid date.
	 * @param day day of the month
	 * @param month month (January=1, February=2, etc.)
	 * @param year year (please note that @class cannot represent years preceding 1900)
	 * @return true for existing dates, false for invalid dates (such as February 31 etc.)
	 */
	public static boolean isValid(int day, int month, int year) {

		if (year < 1900 || day < 1 || month < 1 || month > 12) {
			return false;
		}

		if (month == 2) {
			if (isLeap(year) || year == 1900) {
				if (day > 29) {
					return false;
				} else {
					return true;
				}
			} else {
				if (day > 28) {
					return false;
				} else {
					return true;
				}
			}
		}

		if (day > Date.DAYS_IN_MONTH[month - 1]) {
			return false;
		} else {
			return true;
		}
	}

	/**
	 * This is GetSerial function
	 * @param day day of the month
	 * @param month month (January=1, February=2, etc.)
	 * @param year year (1900 and higher)
	 * @return Function returns a serial number representing the given day.
	 * No check for validity of the date is performed, so even for non-sensical dates
	 * like February-31 a serial number would be returned.
	 */
	public static int getSerial(int day, int month, int year) {

		int n;
		n = (year - 1900) * 365 + (year == 1900 ? 1 : 2);
		// Excel assigns serial number 1 to January 1 1900,
		//   2 to January 2 1900, etc.
		// The trouble is, Feb-29-1900 isn't actually a valid date
		//   (1900 was not a leap year) but Excel assumes 1900 was a leap year
		//   and therefore Feb-29-1900 was made a correct date.
		//
		//   So let's follow this faulty convention and assume that this
		//   date existed so that we stay compliant with Excel numbering.
		// From March-1-1900 on is everything all right.

		n = n + (year - 1901) / 4;
		n = n - (year - 1901) / 100;
		n = n + (year - 1601) / 400;
		n = n + Date.DAYS_IN_MONTH_CUM[month - 1] + day - 1;
		if ((isLeap(year) || year == 1900) && month > 2) {
			n++;
		}

		return n;
	}

	/**
	 * Function getWeekDay returns the number of day in the week.
	 * @return Function getWeekDay returns the number of day in the week. 
	 * Monday is assigned number 1, Tuesday 2 ... Sunday 7.
	 */
	public int getWeekDay() {
		return (serial - 2) % 7 + 1;
	}

	public Date getNext() {
		return getValueOf(serial + 1);
	}

	public Date getPrev() {
		if (serial == 1) {
			throw new IllegalArgumentException("1.1.1900 is the first date, previous dates cannot be produced.");
		}

		return getValueOf(serial - 1);
	}

	public static int lastDayOfMonth(int month, int year) {
		if (month > 12 || month < 1) {
			throw new IllegalArgumentException("Non-existing month: " + month);
		}
		return (Date.DAYS_IN_MONTH[month - 1] +
				((month == 2 && (Date.isLeap(year) || year == 1900)) ? 1 : 0));
	}

	public int LastDayOfMonth() {
		return Date.lastDayOfMonth(month, year);
	}

	public Date addMonths(int n) {

		int aday = this.day;
		int amonth = this.month;
		int ayear = this.year;

		amonth += n % 12;
		ayear += n / 12;
		if (amonth > 12) {
			amonth -= 12;
			ayear++;
		} else {
			if (amonth < 1) {
				amonth += 12;
				ayear--;
			}
		}

		if (aday > DAYS_IN_MONTH[amonth - 1]) {
			if (Date.isLeap(ayear) && amonth == 2) {
				aday = 29;
			} else {
				aday = DAYS_IN_MONTH[amonth - 1];
			}
		}

		return Date.getValueOf(aday, amonth, ayear);
	}

	public static Date EDate(Date date, int n) {
		return date.addMonths(n);
	}

	public Date endOfMonth(int n) {
		int amonth = this.month;
		int ayear = this.year;

		if (n != 0) {
			amonth += n % 12;
			ayear += n / 12;
			if (amonth > 12) {
				amonth -= 12;
				ayear++;
			} else {
				if (amonth < 1) {
					amonth += 12;
					ayear--;
				}
			}
		}
		return endOfMonth(amonth, ayear);
	}

	public static Date endOfMonth(int month, int year) {
		return Date.getValueOf(lastDayOfMonth(month, year), month, year);
	}
}
