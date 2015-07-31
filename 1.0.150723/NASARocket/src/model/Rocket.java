/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package model;

import java.util.Date;

/**
 *
 * @author Sandaruwan
 */
public interface Rocket extends Comparable<Rocket> {

    public Pilot getPilot();

    public void setPilot(Pilot pilot);

    public int getId();

    public void setId(int id);

    public String getRocketName();

    public void setRocketName(String rocketName);

    public long getDistance();

    public void setDistance(long distance);

    public Date getTimeStarted();

    public void setTimeStarted(Date timeStarted);

}
