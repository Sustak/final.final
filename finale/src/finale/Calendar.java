package finale;

import finale.utils.Date;
import java.util.Arrays;
import java.util.SortedSet;
import java.util.TreeSet;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 * @see {%link http://www.tondering.dk/claus/cal/node3.html#SECTION003131000000000000000}
 */
public class Calendar {

	private final boolean[] weekHolidays;
	private final int[] weekWorkDayNumber;
	private final DateY[] annualHolidays;
	private final boolean isEasterFridayAHoliday;
	private final boolean isEasterMondayAHoliday;
	private SortedSet<Date> holidays;
	private int yearFrom;
	private int yearTo;

	public static class DateY {

		private final int day;
		private final int month;

		public DateY(int day, int month) {
			this.day = day;
			this.month = month;
		}

		public int getDay() {
			return day;
		}

		public int getMonth() {
			return month;
		}
	}

	private Calendar() {
		this(null, null, null, false, false);
	}

	private Calendar(int[] weekHolidays, Date[] holidays, DateY[] annualHolidays,
			boolean isEasterFridayAHoliday, boolean isEasterMondayAHoliday) {

		if (holidays == null) {
			this.holidays = new TreeSet<Date>();
		} else {
			this.holidays = new TreeSet<Date>(Arrays.asList(holidays));
		}
		yearFrom = -1;
		yearTo = -1;

		if (weekHolidays != null) {
			this.weekHolidays = new boolean[7];
			this.weekWorkDayNumber = new int[7];
			for (int d : weekHolidays) {
				if (d < 0 || d > 6) {
					throw new IllegalArgumentException("Illegal week day index (" + d + ")");
				}
				this.weekHolidays[d - 1] = true;
			}
			int wd = 0;
			for (int i = 0; i < 7; i++) {
				if (!this.weekHolidays[i]) {
					wd++;
				}
				this.weekWorkDayNumber[i] = wd;
			}
		} else {
			this.weekHolidays = null;
			this.weekWorkDayNumber = null;
		}

		this.annualHolidays = annualHolidays;

		this.isEasterFridayAHoliday = isEasterFridayAHoliday;
		this.isEasterMondayAHoliday = isEasterMondayAHoliday;
	}

	public boolean isHoliday(Date date) {

		// first we check that the date is a week day
		if (isWeekDay(date.getWeekDay()) == false) {
			return true;
		}

		// then make sure that holidays for the current year are generated
		checkYear(date.getYear());

		// look up the holiday list
		return holidays.contains(date);
	}

	public Date getNextWorkDay(Date date) {

		date = Date.getValueOf(date.toInt() + 1);
		while (isHoliday(date)) {
			date = Date.getValueOf(date.toInt() + 1);
		}
		return date;

	}

	public Date getPrevWorkDay(Date date) {

		date = Date.getValueOf(date.toInt() - 1);
		while (isHoliday(date)) {
			date = Date.getValueOf(date.toInt() + 1);
		}
		return date;

	}

	public Date getWorkDay(Date date, int n) {

		Date dt = date;
		if (n >= 0) {
			while (isHoliday(dt)) {
				dt = Date.getValueOf(dt.toInt() + 1);
			}
			if (!dt.equals(date)) {
				n--;
			}
			if (n <= 0) {
				return dt;
			}
			int nWorkDaysPerWeek;
			if (weekHolidays == null) {
				nWorkDaysPerWeek = 5;
			} else {
				nWorkDaysPerWeek = weekWorkDayNumber[6];
			}

			int end = dt.toInt() + n / nWorkDaysPerWeek * 7;
			int dayOfWeek = dt.getWeekDay();
			int remainder = n % nWorkDaysPerWeek;
			if (weekHolidays == null) {
				end += remainder;
				dayOfWeek += remainder;
				if (dayOfWeek > 5) {
					end += 2;
				}
			} else {
				while (remainder > 0) {
					remainder--;
					end++;
					dayOfWeek = dayOfWeek == 7 ? 1 : dayOfWeek + 1;
					while (!isWeekDay(dayOfWeek)) {
						end++;
						dayOfWeek = dayOfWeek == 7 ? 1 : dayOfWeek + 1;
					}
				}
			}

			Date endDate = Date.getValueOf(end);
			checkYear(endDate.getYear());
			SortedSet<Date> holidaysInBetween = holidays.subSet(dt, endDate);
			int nHolidaysInBetween = holidaysInBetween.size();
			if (nHolidaysInBetween > 0) {
				return getWorkDay(endDate, nHolidaysInBetween);
			}

			return endDate;
		} else {
			while (isHoliday(dt)) {
				dt = Date.getValueOf(dt.toInt() - 1);
			}
			if (!dt.equals(date)) {
				n++;
			}
			if (n == 0) {
				return dt;
			}
			int nWorkDaysPerWeek;
			if (weekHolidays == null) {
				nWorkDaysPerWeek = 5;
			} else {
				nWorkDaysPerWeek = weekWorkDayNumber[6];
			}

			int end = dt.toInt() + n / nWorkDaysPerWeek * 7;
			int dayOfWeek = dt.getWeekDay();
			int remainder = -(n % nWorkDaysPerWeek);
			if (weekHolidays == null) {
				end -= remainder;
				dayOfWeek -= remainder;
				if (dayOfWeek < 1) {
					end -= 2;
				}
			} else {
				while (remainder > 0) {
					remainder--;
					end--;
					dayOfWeek = dayOfWeek == 1 ? 7 : dayOfWeek - 1;
					while (!isWeekDay(dayOfWeek + 1)) {
						end++;
						dayOfWeek = dayOfWeek == 1 ? 7 : dayOfWeek - 1;
					}
				}
			}

			Date endDate = Date.getValueOf(end);
			checkYear(endDate.getYear());
			SortedSet<Date> holidaysInBetween = holidays.subSet(endDate, dt);
			int nHolidaysInBetween = holidaysInBetween.size();
			if (nHolidaysInBetween > 0) {
				return getWorkDay(endDate, -nHolidaysInBetween);
			}

			return endDate;
		}
	}

	public boolean isWeekDay(int day) {
		if (day < 1 || day > 7) {
			throw new IllegalArgumentException("Illegal week day index (" + day + ")");
		}

		if (weekHolidays == null) {
			return day <= 5 ? true : false;
		}
		return !weekHolidays[day - 1];
	}

	private void checkYear(int year) {
		if (yearFrom == -1 || yearTo == -1) {
			addHolidaysForYear(year);
			yearFrom = year;
			yearTo = year;
			return;
		}

		int y;
		if (year < yearFrom) {
			for (y = yearFrom - 1; y >= year; y--) {
				addHolidaysForYear(y);
			}
			yearFrom = year;
		} else if (year > yearTo) {
			for (y = yearTo + 1; y <= year; y++) {
				addHolidaysForYear(y);
			}
			yearTo = year;
		}
	}

	private void addHolidaysForYear(int year) {
		if (annualHolidays != null) {
			for (DateY dy : annualHolidays) {
				holidays.add(Date.getValueOf(dy.getDay(), dy.getMonth(), year));
			}
		}
		if (isEasterFridayAHoliday) {
			holidays.add(Calendar.getEasterFriday(year));
		}
		if (isEasterMondayAHoliday) {
			holidays.add(Calendar.getEasterMonday(year));
		}
	}

	public static Calendar getCalendar(String calendarName) {

		DateY[] annHolidays = null;
		boolean easterFriday = false;
		boolean easterMonday = false;

		if (calendarName.toUpperCase().equals("ITALY-DEFAULT")) {
			annHolidays = new DateY[]{
						new DateY(1, 1),
						new DateY(6, 1),
						new DateY(1, 5),
						new DateY(2, 6),
						new DateY(15, 8),
						new DateY(1, 11),
						new DateY(8, 12),
						new DateY(24, 12),
						new DateY(25, 12),
						new DateY(26, 12),
						new DateY(31, 12)
					};

			easterFriday = true;
			easterMonday = true;
		} else if (calendarName.toUpperCase().equals("TARGET")) {
			annHolidays = new DateY[]{
						new DateY(1, 1),
						new DateY(1, 5),
						new DateY(25, 12),
						new DateY(26, 12)
					};
			easterFriday = true;
			easterMonday = true;
		}

		return new Calendar(null, null, annHolidays, easterFriday, easterMonday);
	}

	public static Date getEasterSunday(int year) {
		int goldenNumber = year % 19 + 1;
		int julianEpact = (11 * (goldenNumber - 1)) % 30;
		int century = (year / 100) + 1;
		int s = 3 * century / 4;
		int l = (8 * century + 5) / 25;
		int gregorianEpact = julianEpact - s + l + 8;
		int epact = gregorianEpact % 30;

		Date fullMoon;
		if (epact < 24) {
			int april13 = Date.getValueOf(13, 4, year).toInt();
			fullMoon = Date.getValueOf(april13 - epact);
		} else if (epact == 24) {
			fullMoon = Date.getValueOf(18, 4, year);
		} else if (epact > 25) {
			int april17 = Date.getValueOf(17, 4, year).toInt();
			fullMoon = Date.getValueOf(april17 - epact + 26);
		} else if (goldenNumber > 11) {
			fullMoon = Date.getValueOf(17, 4, year);
		} else {
			fullMoon = Date.getValueOf(18, 4, year);
		}
		int weekDayOfFullMoon = fullMoon.getWeekDay();
		Date easterSunday = Date.getValueOf(fullMoon.toInt() +
				7 - weekDayOfFullMoon % 7);
		return easterSunday;
	}

	public static Date getEasterFriday(int year) {
		Date easterSunday = getEasterSunday(year);
		return Date.getValueOf(easterSunday.toInt() - 2);
	}

	public static Date getEasterMonday(int year) {
		Date easterSunday = getEasterSunday(year);
		return Date.getValueOf(easterSunday.toInt() + 1);
	}
}
