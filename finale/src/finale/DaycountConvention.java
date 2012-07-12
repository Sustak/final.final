/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package finale;

/**
 *
 * @author Šesták
 */
public enum DaycountConvention {

    US_30_360, // 0
    ACT_ACT, // 1
    ACT_360, // 2
    ACT_365, // 3
    ISMA_30_360, // 4
    ISMA_30_365, //5 
    ISMA_30_ACT, // 6
    US_30_365, // 7
    US_30_ACT, // 8
    NL_365, // 9
    NL_360, // 10
    NL_ACT, // 11,

    ISMA_30_360_NON_EOM, // 100
    ACT_ACT_NON_EOM, // 101
    ACT_360_NON_EOM, // 102
    ACT_365_NON_EOM, // 103
    US_30_360_NON_EOM, // 104
    ISMA_30_365_NON_EOM, //105 
    ISMA_30_ACT_NON_EOM, // 106
    US_30_365_NON_EOM, // 107
    US_30_ACT_NON_EOM, // 108
    NL_365_NON_EOM, // 109
    NL_360_NON_EOM, // 110
    NL_ACT_NON_EOM; // 111
    public final static int NON_EOM_BASE = 100;
    public final static DaycountConvention NON_EOM_START = ISMA_30_360_NON_EOM;

    public boolean isNonEOM() {
        return ordinal() >= NON_EOM_START.ordinal();
    }

    public int toInt() {
        int val = ordinal();
        int nonEomStart = NON_EOM_START.ordinal();

        if (val >= nonEomStart) {
            return val - nonEomStart + NON_EOM_BASE;
        } else {
            return val;
        }
    }

    public static DaycountConvention toDaycountConvention(int daycountCode) {
        IllegalArgumentException ex = new IllegalArgumentException("Invalid daycount code");
        int nonEomStart = NON_EOM_START.ordinal();

        try {
            if (daycountCode < NON_EOM_BASE) {
                if (daycountCode >= nonEomStart) {
                    throw ex;
                } else {
                    return values()[daycountCode];
                }
            } else {
                daycountCode -= NON_EOM_BASE;
                daycountCode += nonEomStart;
                if (daycountCode >= values().length) {
                    throw ex;
                }
                return values()[daycountCode];
            }
        } catch (Exception e) {
            throw ex;
        }
    }

    /** This function converts Daycount Convention with the NON_EOM modifier
     * to the "standard" (or "EOM") version.
     * @param dc
     * @return This function converts Daycount Convention with the NON_EOM modifier
     * to the "standard" (or "EOM") version. Example: ACT_ACT_NON_EOM is
     * transformed to ACT_ACT.
     */
    public static DaycountConvention toDaycountConventionEOM(DaycountConvention dc) {
        if (dc.isNonEOM()) {
            int daycountCode = dc.toInt();
            daycountCode -= NON_EOM_BASE;
            return DaycountConvention.toDaycountConvention(daycountCode);
        } else {
            return dc;
        }
    }

    public static DaycountConvention toDaycountConvention(String convention) {
        String convUnderscore = convention.replace('/', '_').replace('-', '_').replace(' ', '_');
        return DaycountConvention.valueOf(convUnderscore);
    }

}
