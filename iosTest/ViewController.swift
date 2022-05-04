//
//  ViewController.swift
//  iosTest
//
//  Created by tridiak on 29/04/22.
//

import UIKit
import UniformTypeIdentifiers
import imgSizeFWIOS

class ViewController: UIViewController, UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIDocumentBrowserViewControllerDelegate {

	override func viewDidLoad() {
		super.viewDidLoad()
		// Do any additional setup after loading the view.
	}


	@IBAction func PickImage(_ sender: UIButton) {
		let documentPicker = UIDocumentBrowserViewController(forOpening: [.image])
		documentPicker.delegate = self
		documentPicker.allowsDocumentCreation = false
		
		// Present the document picker.
		present(documentPicker, animated: true, completion: nil)
	}
	
	@IBOutlet weak var sizeTV: UITextView!
	
	func documentBrowser(_ controller: UIDocumentBrowserViewController, didPickDocumentsAt documentURLs: [URL]) {
		
		var isl = ImgSizeList()
		var s = ""
		for u in documentURLs {
			if u.startAccessingSecurityScopedResource() {
				let res = GetImageSizeC(u.path, &isl)
				if res >= 0 {
					print("{\(u.path) : \(isl.first.w),\(isl.first.h)}")
					s += "{\(u.path) : \(isl.first.w),\(isl.first.h)}\n"
				}
				else {
					print("\(u.path) : Not an image file or an error occurred")
					s += "\(u.path) : Not an image file or an error occurred"
				}
				
				u.stopAccessingSecurityScopedResource()
			}
		}
		sizeTV.text = s
		controller.dismiss(animated: true)
		
	}
}

