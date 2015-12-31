//
//  main.swift
//  BleTest
//
//  Created by Anthony Abbot on 27/12/2015.
//  Copyright © 2015 Anthony Abbot. All rights reserved.
//

import Cocoa
import IOBluetooth
import Foundation

class BlueDelegate : IOBluetoothDeviceInquiryDelegate
{
    @objc func deviceInquiryComplete(sender: IOBluetoothDeviceInquiry, error: IOReturn, aborted: Bool)
    {
        aborted
        print("called")
        let devices = sender.foundDevices()
        for device : AnyObject in devices
        {
            if let thingy = device as? IOBluetoothDevice
            {
                thingy.getAddress()
            }
        }
    }
}

var delegate = BlueDelegate()
var inquiry = IOBluetoothDeviceInquiry(delegate: delegate)
inquiry.start()




print("Hello, World!")

