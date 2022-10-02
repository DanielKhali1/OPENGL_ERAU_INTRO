(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
})(window,document,'script','dataLayer','GTM-P8VPTMR');

window.dataLayer = window.dataLayer || [];

/*EvaluationKIT START*/var evalkit_jshosted = document.createElement('script');evalkit_jshosted.setAttribute('defer', 'defer');evalkit_jshosted.setAttribute('type', 'text/javascript');evalkit_jshosted.setAttribute('src', 'https://erau.evaluationkit.com/canvas/js');document.getElementsByTagName('head')[0].appendChild(evalkit_jshosted);/*EvaluationKIT END*/

if (typeof onElementRendered === 'undefined') {
	function onElementRendered(selector, cb, _attempts) {
		var el = $(selector);
		_attempts = ++_attempts || 1;
		if (el.length) return cb(el); //if (_attempts == 60) return;
		setTimeout(function() {
			onElementRendered(selector, cb, _attempts);
		}, 250);
	}
}

$(document).ready(function() {

	function pad(num, size) {
		num = num.toString();
		while (num.length < size) num = "0" + num;
		return num;
	}

	onElementRendered('#faa_button_holder', function(el) {
		var $btnholder = $('#faa_button_holder');

		if ($btnholder.length) {
			$btnholder.append('<button type="button" class="btn btn-success" id="faa_certificate_button">Print your Certificate!</button>');

			var completeCourse = function () {
				$.ajax({
					type: "POST",
					url: "https://webforms.erau.edu/common/services/canvas/faaTrust.cfc?method=completeCourse&userId=" + ENV.current_user.id + "&completionToken=ERAU" + pad(ENV.current_user.id, 11) + "&apiKey=aea45701-9d9f-4822-be5a-58a757b7fc5c",
					dataType: "jsonp",
					crossDomain: true
				})
				.done(function (response, status, XHR) {
					if (response.MESSAGE == "success") {
						console.log("Course Complete indication sent: " + status);
					}
				})
				.fail(function (response, status, XHR) {
					console.log("Course Complete indication NOT sent due to a failure: " + status);
				});
			};
		
			$("#faa_certificate_button").click(function () {
				var name = ENV.current_user.display_name;
				var userid = pad(ENV.current_user.id, 11);
				var dateObj = new Date();
				var month = pad(dateObj.getUTCMonth() + 1, 2); //months from 1-12
				var day = pad(dateObj.getUTCDate(), 2);
				var year = dateObj.getUTCFullYear();
		
				var newdate = month + "/" + day + "/" + year;
			
				var b = `
				<link rel="preconnect" href="https://fonts.gstatic.com">
				<link href="https://fonts.googleapis.com/css2?family=Gothic+A1:wght@400;700&display=swap" rel="stylesheet">
				
				<style>
				* {
					-webkit-print-color-adjust: exact !important;   /* Chrome, Safari */
					color-adjust: exact !important;                 /*Firefox*/
				}
	
				@page {
					size: letter landscape;
						/* margin: 25.4mm 12.7mm 25.4mm 12.7mm; */
						/* margin: 8mm 12.7mm 25.4mm 12.7mm; */
						margin: 0mm 12.7mm 5mm 12.7mm;
				
						@top-center {
							content: element(header);
						}
				
						@bottom-center {
							content: element(footer);
						}
					}
	
					.faa-cert-wrap {
						margin-top: 40px;
						font-family: 'Gothic A1', sans-serif;
						font-size: 2.125vw;
					}
				
					.faa-cert-wrap .pageHeader {
						/* margin-top: 15px; */
						margin-top: 0px;
						position: running(header);
					}
				
					.faa-cert-wrap .pageFooter {
						position: running(footer);
					}
				
					.faa-cert-wrap .keepTogether {
						page-break-inside: avoid;
						background-image: url("https://erau.instructure.com/courses/130339/files/26967220/preview?instfs=true") !important;
						background-size: 100vw;
						padding: 21% 0 13% 7%;
						background-repeat: no-repeat;
					}
				
					.faa-cert-wrap .alwaysBreak {
						page-break-after: always;
					}
				
					.faa-cert-wrap .page-2 {
						margin-top: 60px;
						padding: 5% 0 20% 7%;
	
						background-image: url("https://erau.instructure.com/courses/130339/files/26967221/preview?instfs=true") !important;
					}
				
					.faa-cert-wrap h1 {
						margin-top: 10px;
						margin-bottom: 20px;
						font-size: 4vw;
						font-weight: bold;
					}
				
					.faa-cert-wrap h2 {
						padding: 0px;
						margin-top: 0;
						margin-bottom: 0;
						font-size: 3vw;
						font-weight: bold;
					}
			
					.faa-cert-wrap .logo {
						display: inline-block;
					}
				
					.faa-cert-wrap .user-field {
						margin-top: 1%;
						font-size: 2vw;
					}
				
					.faa-cert-wrap li {
						padding: 1px;
					}
				
					.faa-cert-wrap .terms {
						font-size: 1.75vw;
					}
				</style>
				
				<div class="faa-cert-wrap">
					<div class="keepTogether">
						<h1>
							The Recreational UAS Safety Test (TRUST)<br />
							Completion Certificate
						</h1>
				
						<h2>Name:</h2>
						<p id="faa_certificat_name" class="user-field">${name}</p>
				
						<h2>Authentication Token:</h2>
						<p id="faa_certificat_token" class="user-field">ERAU${userid}</p>
				
						<h2>Issued By:</h2>
						<p class="user-field">Embry-Riddle Aeronautical University <i>on</i> <span id="faa_certificat_date">${newdate}</span></p>
					</div>
				
					<div class="alwaysBreak"></div>
				
					<!--- Page 2 --->
					<div class="keepTogether page-2">
						<br />
						<br />
						<strong>
							For Recreational Flyers operating under the exception for limited<br />
							recreational operations of unmanned aircraft (Title 49 of the United States Code<br />
							(49 U.S.C.) &sect; 44089), this document is evidence that the holder has passed the<br />
							aeronautical knowledge and safety test as required by 49 U.S.C &sect; 44809(a)(7).
						</strong>
				
						<ul class="terms">
							<li>Register your drone if it weighs more than 0.55 pounds</li>
							<li>Fly only for recreational purposes</li>
							<li>
								Operate in accordance with or within a Community-Based Organization's<br />
								(CBO's) safety guidelines
							</li>
							<li>Always fly within visual line of sight</li>
							<li>Do not interfere and give way to any manned aircraft</li>
							<li>Obtain authorization before flying in controlled airspace</li>
							<li>Do not fly higher than 400 feet in uncontrolled airspace</li>
						</ul>
				
						<strong>
						Recreational flyers must make this certificate available to Federal Aviation<br />
						Administration (FAA) or Law Enforcement officials upon request.<br />
						<br />
						For more information, visit faa.gov/uas.
						</strong>
					</div>
				</div>`
				var printWin = window.open('', '', 'left=0,top=0,width=' + $(document).width() + ',height=' + $(document).height() + ',toolbar=0,scrollbars=0,status=0');
				printWin.document.write(b);
				printWin.document.close();
				printWin.focus();
				setTimeout(function () {
					printWin.print();
					printWin.close();
					completeCourse();
				}, 1500);
			});	
		}
	}, 4);
	
	///////////////////////////////////////////////////////////
	// Add My Advisors link to Account Nav (Global Nav Tray) //
	///////////////////////////////////////////////////////////
	$(document.body).on('click', '#global_nav_profile_link', function(){
		// Take a brief pause
		setTimeout(function(){
			var navigationTray = $('.tray-with-space-for-global-nav');
			var navigationTrayOpen = false;
			
			if (navigationTray.length) {
				var navigationTrayOpen = true;
			}
	
			if (navigationTrayOpen === true) {
				// Wait for Global Nav Tray to open
				onElementRendered('.navigation-tray-container.profile-tray', function(){
					var webformsuri = (document.location.host.indexOf("erau.instructure") >= 0) ? "webforms.erau.edu" : "webformstest.erau.edu";
					var navLinks = $('.fOyUs_bGBk.fOyUs_UeJS.fClCc_bGBk.fClCc_fLbg');

					// Create Loading indicator
					var myAdvisorLoader = $(document.createElement('li'));
					myAdvisorLoader.html('My Advisors <style>@keyframes loader-spin {0% {transform: rotate(0deg);}100% {transform: rotate(360deg);}}</style><div style="display: inline-block; position: relative; top: 4px; margin: 0; border: 3px solid #ddd; border-top: 3px solid #3a6fcd; border-radius: 50%; width: 16px; height: 16px; animation: loader-spin 1s linear infinite;"></div>');
					navLinks.append(myAdvisorLoader);

					var studentProfile = $.getJSON('/api/v1/users/' + ENV.current_user_id + '/profile');

					studentProfile.success(function(profile){
						var studentEmail = profile.login_id;
						var erauId = studentEmail.split('@')[0];
					//	var erauId = "colec24";
					//	var erauId = "holguinv";
					//	var erauId = "delatop1";
					//	var erauId = "edwarr21";
					//	var erauId = "romeroc7";
					//	var erauId = "davisk56";
					var myAdvisorInfo = $.getJSON("https://" + webformsuri + "/common/services/canvas/advisors.cfc?method=getStudentAdvisors&apikey=fa5d7116-38af-490c-a230-1b8ada733a27&uid=" + erauId);

						myAdvisorInfo.success(function(data){
							var myAdvisorLink = $(document.createElement('li'));

							if ( data.length ) {

								myAdvisorLink.html(
									$(document.createElement('a'))
										.css({
											display: 'block'
										})
										.attr('aria-label', 'My Advisors')
									//	.attr('onclick', "ga('send', 'event', 'student-tray', 'link', 'Email Advisor', 1);")
										.attr('href', '#')
										.html('My Advisors <span class="badge">' + data.length + '</span>')
								);

								// Add Advisor link / modal to Main navigation
								var advisor_modal = document.createElement( 'div' );

								var modalContents = "";

								data.sort(function(a, b) {
									let pa = a.PLAN_TYPE_DESCR.toLowerCase() + " " + a.ACAD_PLAN_DESCR.toLowerCase(),
									pb = b.PLAN_TYPE_DESCR.toLowerCase() + " " + b.ACAD_PLAN_DESCR.toLowerCase();

									if (pa < pb) {
										return -1;
									}
									if (pa > pb) {
										return 1;
									}
									return 0;
								});

								var previousMajor = "";

								for (var i = 0; i < data.length; i++) {
								//	let advisorFirstName = data[i].ER_ADVISOR_NAME.split(' ')[0] ?? '';
								//	var msBookingButton = (data[i].URL != "") ? '<br><a class="btn btn-small btn-primary" target="_blank" style="margin-top: 8px;" href="'+ data[i].URL + '">Schedule an Appointment with ' + advisorFirstName + '</a>' : '';
									var msBookingButton = (data[i].URL != "") ? '<br><a class="btn btn-small btn-primary" target="_blank" style="margin-top: 8px;" href="'+ data[i].URL + '">Make Appointment</a>' : '';
									var areaCode = data[i].STDNT_ADVSR_PHONE.split('/')[0] ?? '';
									var prefixAndNumber = data[i].STDNT_ADVSR_PHONE.split('/')[1] ?? '';

									if (data[i].ACAD_PLAN_DESCR != previousMajor && i != 0) modalContents += '<hr />';
									var planType = ( data[i].PLAN_TYPE_DESCR != "" ) ? data[i].PLAN_TYPE_DESCR : "Program"
									if (data[i].ACAD_PLAN_DESCR != previousMajor) modalContents += '<h4 style="font-weight: bold;">' + planType + ': ' + data[i].ACAD_PLAN_DESCR + '</h4>';
									modalContents += '\
										<p><b>' + data[i].ER_ADVISOR_NAME + '</b> (' + data[i].ADVISOR_ROLE_DESCR + ')<br>\
										Email: <a href="mailto:' + data[i].STDNT_ADVSR_EMAIL + '">' + data[i].STDNT_ADVSR_EMAIL + '</a><br>\
										Phone: (' + areaCode + ') ' + prefixAndNumber + '\
										' + msBookingButton + '\
										</p>\
										'
									previousMajor = data[i].ACAD_PLAN_DESCR;
								}
								$( advisor_modal ).html( modalContents );
								myAdvisorLink.click(function() {
									$( this ).blur();
									$( advisor_modal ).dialog({
										title: "My Advisors",
										width: 400,
										resizable: !1
									});
									event.preventDefault();
									event.stopPropagation();
								});
								
							}
							else {
								myAdvisorLink.html('My Advisors <span class="badge">0</span>');
							}
							navLinks.append(myAdvisorLink);
							myAdvisorLoader.remove();
						});
					});
				}, 6*1000);
			}
		}, 50);
	});

	if ($.inArray('admin', ENV.current_user_roles) === -1) {
		// ONLY FOR NON-ADMINS
		
	} else {
		// ONLY FOR ADMINS
		// These may not be needed because we removed the corresponding CSS on 041221
		$('#section-tabs a[href*="/external_tools/96"]').attr("style", "display: block !important;");
		$('#section-tabs a[href*="/external_tools/95"]').attr("style", "display: block !important;");
	}

	onElementRendered('#pairing-code', function(el) {
		$('#pairing-code').before("<div style=\"padding: 5px;\">By sharing a paring code with someone, you agree to allow them to connect with you and view confidential academic records for the course, including grades you have earned.</div>");
	}, 4);

	// Rename EagleVision to EagleVision Zoom
	$('#section-tabs a[href*="/external_tools/72898"]').text("EagleVision Zoom").attr("title", "EagleVision Zoom");
	// Same change in Navigation > Settings
	if ($('#nav_edit_tab_id_context_external_tool_72898').length) {
		$('#nav_edit_tab_id_context_external_tool_72898').html($('#nav_edit_tab_id_context_external_tool_72898').html().replace(/EagleVision/g, "EagleVision Zoom")).attr("aria-label", "EagleVision Zoom");
	}
	if (window.location.pathname.search("external_tools/72898") > 0) {
		window.document.title = "EagleVision Zoom";
	}

	// Rename BigBlueButton to Conferences
	if ($('#section-tabs a[href*="/conferences"].conferences').length) {
		$('#section-tabs a[href*="/conferences"].conferences').html($('#section-tabs a[href*="/conferences"].conferences').html().replace(/BigBlueButton [(]Formerly Conferences[)]/g, "Conferences")).attr("aria-label", "Conferences");
	}
	// Same change in Navigation > Settings
	if ($('#nav_edit_tab_id_12').length) {
		$('#nav_edit_tab_id_12').html($('#nav_edit_tab_id_12').html().replace(/BigBlueButton [(]Formerly Conferences[)]/g, "Conferences")).attr("aria-label", "Conferences");
	}

	// Idenftify test or beta environment
	if (window.location.hostname === "erau.test.instructure.com") {
		$(document.body).addClass('erau-test');
	} else if (window.location.hostname === "erau.beta.instructure.com") {
		$(document.body).addClass('erau-beta');
	}

	// Customized jQuery UI download - only accordions and tabs
	/*! jQuery UI - v1.12.1 - 2017-06-20
	* http://jqueryui.com
	* Includes: widget.js, keycode.js, unique-id.js, widgets/accordion.js, widgets/tabs.js
	* Copyright jQuery Foundation and other contributors; Licensed MIT */
	
	if ( typeof jQuery.ui == 'undefined') {
		(function(t){"function"==typeof define&&define.amd?define(["jquery"],t):t(jQuery)})(function(t){t.ui=t.ui||{},t.ui.version="1.12.1";var e=0,i=Array.prototype.slice;t.cleanData=function(e){return function(i){var s,n,o;for(o=0;null!=(n=i[o]);o++)try{s=t._data(n,"events"),s&&s.remove&&t(n).triggerHandler("remove")}catch(a){}e(i)}}(t.cleanData),t.widget=function(e,i,s){var n,o,a,r={},l=e.split(".")[0];e=e.split(".")[1];var h=l+"-"+e;return s||(s=i,i=t.Widget),t.isArray(s)&&(s=t.extend.apply(null,[{}].concat(s))),t.expr[":"][h.toLowerCase()]=function(e){return!!t.data(e,h)},t[l]=t[l]||{},n=t[l][e],o=t[l][e]=function(t,e){return this._createWidget?(arguments.length&&this._createWidget(t,e),void 0):new o(t,e)},t.extend(o,n,{version:s.version,_proto:t.extend({},s),_childConstructors:[]}),a=new i,a.options=t.widget.extend({},a.options),t.each(s,function(e,s){return t.isFunction(s)?(r[e]=function(){function t(){return i.prototype[e].apply(this,arguments)}function n(t){return i.prototype[e].apply(this,t)}return function(){var e,i=this._super,o=this._superApply;return this._super=t,this._superApply=n,e=s.apply(this,arguments),this._super=i,this._superApply=o,e}}(),void 0):(r[e]=s,void 0)}),o.prototype=t.widget.extend(a,{widgetEventPrefix:n?a.widgetEventPrefix||e:e},r,{constructor:o,namespace:l,widgetName:e,widgetFullName:h}),n?(t.each(n._childConstructors,function(e,i){var s=i.prototype;t.widget(s.namespace+"."+s.widgetName,o,i._proto)}),delete n._childConstructors):i._childConstructors.push(o),t.widget.bridge(e,o),o},t.widget.extend=function(e){for(var s,n,o=i.call(arguments,1),a=0,r=o.length;r>a;a++)for(s in o[a])n=o[a][s],o[a].hasOwnProperty(s)&&void 0!==n&&(e[s]=t.isPlainObject(n)?t.isPlainObject(e[s])?t.widget.extend({},e[s],n):t.widget.extend({},n):n);return e},t.widget.bridge=function(e,s){var n=s.prototype.widgetFullName||e;t.fn[e]=function(o){var a="string"==typeof o,r=i.call(arguments,1),l=this;return a?this.length||"instance"!==o?this.each(function(){var i,s=t.data(this,n);return"instance"===o?(l=s,!1):s?t.isFunction(s[o])&&"_"!==o.charAt(0)?(i=s[o].apply(s,r),i!==s&&void 0!==i?(l=i&&i.jquery?l.pushStack(i.get()):i,!1):void 0):t.error("no such method '"+o+"' for "+e+" widget instance"):t.error("cannot call methods on "+e+" prior to initialization; "+"attempted to call method '"+o+"'")}):l=void 0:(r.length&&(o=t.widget.extend.apply(null,[o].concat(r))),this.each(function(){var e=t.data(this,n);e?(e.option(o||{}),e._init&&e._init()):t.data(this,n,new s(o,this))})),l}},t.Widget=function(){},t.Widget._childConstructors=[],t.Widget.prototype={widgetName:"widget",widgetEventPrefix:"",defaultElement:"<div>",options:{classes:{},disabled:!1,create:null},_createWidget:function(i,s){s=t(s||this.defaultElement||this)[0],this.element=t(s),this.uuid=e++,this.eventNamespace="."+this.widgetName+this.uuid,this.bindings=t(),this.hoverable=t(),this.focusable=t(),this.classesElementLookup={},s!==this&&(t.data(s,this.widgetFullName,this),this._on(!0,this.element,{remove:function(t){t.target===s&&this.destroy()}}),this.document=t(s.style?s.ownerDocument:s.document||s),this.window=t(this.document[0].defaultView||this.document[0].parentWindow)),this.options=t.widget.extend({},this.options,this._getCreateOptions(),i),this._create(),this.options.disabled&&this._setOptionDisabled(this.options.disabled),this._trigger("create",null,this._getCreateEventData()),this._init()},_getCreateOptions:function(){return{}},_getCreateEventData:t.noop,_create:t.noop,_init:t.noop,destroy:function(){var e=this;this._destroy(),t.each(this.classesElementLookup,function(t,i){e._removeClass(i,t)}),this.element.off(this.eventNamespace).removeData(this.widgetFullName),this.widget().off(this.eventNamespace).removeAttr("aria-disabled"),this.bindings.off(this.eventNamespace)},_destroy:t.noop,widget:function(){return this.element},option:function(e,i){var s,n,o,a=e;if(0===arguments.length)return t.widget.extend({},this.options);if("string"==typeof e)if(a={},s=e.split("."),e=s.shift(),s.length){for(n=a[e]=t.widget.extend({},this.options[e]),o=0;s.length-1>o;o++)n[s[o]]=n[s[o]]||{},n=n[s[o]];if(e=s.pop(),1===arguments.length)return void 0===n[e]?null:n[e];n[e]=i}else{if(1===arguments.length)return void 0===this.options[e]?null:this.options[e];a[e]=i}return this._setOptions(a),this},_setOptions:function(t){var e;for(e in t)this._setOption(e,t[e]);return this},_setOption:function(t,e){return"classes"===t&&this._setOptionClasses(e),this.options[t]=e,"disabled"===t&&this._setOptionDisabled(e),this},_setOptionClasses:function(e){var i,s,n;for(i in e)n=this.classesElementLookup[i],e[i]!==this.options.classes[i]&&n&&n.length&&(s=t(n.get()),this._removeClass(n,i),s.addClass(this._classes({element:s,keys:i,classes:e,add:!0})))},_setOptionDisabled:function(t){this._toggleClass(this.widget(),this.widgetFullName+"-disabled",null,!!t),t&&(this._removeClass(this.hoverable,null,"ui-state-hover"),this._removeClass(this.focusable,null,"ui-state-focus"))},enable:function(){return this._setOptions({disabled:!1})},disable:function(){return this._setOptions({disabled:!0})},_classes:function(e){function i(i,o){var a,r;for(r=0;i.length>r;r++)a=n.classesElementLookup[i[r]]||t(),a=e.add?t(t.unique(a.get().concat(e.element.get()))):t(a.not(e.element).get()),n.classesElementLookup[i[r]]=a,s.push(i[r]),o&&e.classes[i[r]]&&s.push(e.classes[i[r]])}var s=[],n=this;return e=t.extend({element:this.element,classes:this.options.classes||{}},e),this._on(e.element,{remove:"_untrackClassesElement"}),e.keys&&i(e.keys.match(/\S+/g)||[],!0),e.extra&&i(e.extra.match(/\S+/g)||[]),s.join(" ")},_untrackClassesElement:function(e){var i=this;t.each(i.classesElementLookup,function(s,n){-1!==t.inArray(e.target,n)&&(i.classesElementLookup[s]=t(n.not(e.target).get()))})},_removeClass:function(t,e,i){return this._toggleClass(t,e,i,!1)},_addClass:function(t,e,i){return this._toggleClass(t,e,i,!0)},_toggleClass:function(t,e,i,s){s="boolean"==typeof s?s:i;var n="string"==typeof t||null===t,o={extra:n?e:i,keys:n?t:e,element:n?this.element:t,add:s};return o.element.toggleClass(this._classes(o),s),this},_on:function(e,i,s){var n,o=this;"boolean"!=typeof e&&(s=i,i=e,e=!1),s?(i=n=t(i),this.bindings=this.bindings.add(i)):(s=i,i=this.element,n=this.widget()),t.each(s,function(s,a){function r(){return e||o.options.disabled!==!0&&!t(this).hasClass("ui-state-disabled")?("string"==typeof a?o[a]:a).apply(o,arguments):void 0}"string"!=typeof a&&(r.guid=a.guid=a.guid||r.guid||t.guid++);var l=s.match(/^([\w:-]*)\s*(.*)$/),h=l[1]+o.eventNamespace,c=l[2];c?n.on(h,c,r):i.on(h,r)})},_off:function(e,i){i=(i||"").split(" ").join(this.eventNamespace+" ")+this.eventNamespace,e.off(i).off(i),this.bindings=t(this.bindings.not(e).get()),this.focusable=t(this.focusable.not(e).get()),this.hoverable=t(this.hoverable.not(e).get())},_delay:function(t,e){function i(){return("string"==typeof t?s[t]:t).apply(s,arguments)}var s=this;return setTimeout(i,e||0)},_hoverable:function(e){this.hoverable=this.hoverable.add(e),this._on(e,{mouseenter:function(e){this._addClass(t(e.currentTarget),null,"ui-state-hover")},mouseleave:function(e){this._removeClass(t(e.currentTarget),null,"ui-state-hover")}})},_focusable:function(e){this.focusable=this.focusable.add(e),this._on(e,{focusin:function(e){this._addClass(t(e.currentTarget),null,"ui-state-focus")},focusout:function(e){this._removeClass(t(e.currentTarget),null,"ui-state-focus")}})},_trigger:function(e,i,s){var n,o,a=this.options[e];if(s=s||{},i=t.Event(i),i.type=(e===this.widgetEventPrefix?e:this.widgetEventPrefix+e).toLowerCase(),i.target=this.element[0],o=i.originalEvent)for(n in o)n in i||(i[n]=o[n]);return this.element.trigger(i,s),!(t.isFunction(a)&&a.apply(this.element[0],[i].concat(s))===!1||i.isDefaultPrevented())}},t.each({show:"fadeIn",hide:"fadeOut"},function(e,i){t.Widget.prototype["_"+e]=function(s,n,o){"string"==typeof n&&(n={effect:n});var a,r=n?n===!0||"number"==typeof n?i:n.effect||i:e;n=n||{},"number"==typeof n&&(n={duration:n}),a=!t.isEmptyObject(n),n.complete=o,n.delay&&s.delay(n.delay),a&&t.effects&&t.effects.effect[r]?s[e](n):r!==e&&s[r]?s[r](n.duration,n.easing,o):s.queue(function(i){t(this)[e](),o&&o.call(s[0]),i()})}}),t.widget,t.ui.keyCode={BACKSPACE:8,COMMA:188,DELETE:46,DOWN:40,END:35,ENTER:13,ESCAPE:27,HOME:36,LEFT:37,PAGE_DOWN:34,PAGE_UP:33,PERIOD:190,RIGHT:39,SPACE:32,TAB:9,UP:38},t.fn.extend({uniqueId:function(){var t=0;return function(){return this.each(function(){this.id||(this.id="ui-id-"+ ++t)})}}(),removeUniqueId:function(){return this.each(function(){/^ui-id-\d+$/.test(this.id)&&t(this).removeAttr("id")})}}),t.widget("ui.accordion",{version:"1.12.1",options:{active:0,animate:{},classes:{"ui-accordion-header":"ui-corner-top","ui-accordion-header-collapsed":"ui-corner-all","ui-accordion-content":"ui-corner-bottom"},collapsible:!1,event:"click",header:"> li > :first-child, > :not(li):even",heightStyle:"auto",icons:{activeHeader:"ui-icon-triangle-1-s",header:"ui-icon-triangle-1-e"},activate:null,beforeActivate:null},hideProps:{borderTopWidth:"hide",borderBottomWidth:"hide",paddingTop:"hide",paddingBottom:"hide",height:"hide"},showProps:{borderTopWidth:"show",borderBottomWidth:"show",paddingTop:"show",paddingBottom:"show",height:"show"},_create:function(){var e=this.options;this.prevShow=this.prevHide=t(),this._addClass("ui-accordion","ui-widget ui-helper-reset"),this.element.attr("role","tablist"),e.collapsible||e.active!==!1&&null!=e.active||(e.active=0),this._processPanels(),0>e.active&&(e.active+=this.headers.length),this._refresh()},_getCreateEventData:function(){return{header:this.active,panel:this.active.length?this.active.next():t()}},_createIcons:function(){var e,i,s=this.options.icons;s&&(e=t("<span>"),this._addClass(e,"ui-accordion-header-icon","ui-icon "+s.header),e.prependTo(this.headers),i=this.active.children(".ui-accordion-header-icon"),this._removeClass(i,s.header)._addClass(i,null,s.activeHeader)._addClass(this.headers,"ui-accordion-icons"))},_destroyIcons:function(){this._removeClass(this.headers,"ui-accordion-icons"),this.headers.children(".ui-accordion-header-icon").remove()},_destroy:function(){var t;this.element.removeAttr("role"),this.headers.removeAttr("role aria-expanded aria-selected aria-controls tabIndex").removeUniqueId(),this._destroyIcons(),t=this.headers.next().css("display","").removeAttr("role aria-hidden aria-labelledby").removeUniqueId(),"content"!==this.options.heightStyle&&t.css("height","")},_setOption:function(t,e){return"active"===t?(this._activate(e),void 0):("event"===t&&(this.options.event&&this._off(this.headers,this.options.event),this._setupEvents(e)),this._super(t,e),"collapsible"!==t||e||this.options.active!==!1||this._activate(0),"icons"===t&&(this._destroyIcons(),e&&this._createIcons()),void 0)},_setOptionDisabled:function(t){this._super(t),this.element.attr("aria-disabled",t),this._toggleClass(null,"ui-state-disabled",!!t),this._toggleClass(this.headers.add(this.headers.next()),null,"ui-state-disabled",!!t)},_keydown:function(e){if(!e.altKey&&!e.ctrlKey){var i=t.ui.keyCode,s=this.headers.length,n=this.headers.index(e.target),o=!1;switch(e.keyCode){case i.RIGHT:case i.DOWN:o=this.headers[(n+1)%s];break;case i.LEFT:case i.UP:o=this.headers[(n-1+s)%s];break;case i.SPACE:case i.ENTER:this._eventHandler(e);break;case i.HOME:o=this.headers[0];break;case i.END:o=this.headers[s-1]}o&&(t(e.target).attr("tabIndex",-1),t(o).attr("tabIndex",0),t(o).trigger("focus"),e.preventDefault())}},_panelKeyDown:function(e){e.keyCode===t.ui.keyCode.UP&&e.ctrlKey&&t(e.currentTarget).prev().trigger("focus")},refresh:function(){var e=this.options;this._processPanels(),e.active===!1&&e.collapsible===!0||!this.headers.length?(e.active=!1,this.active=t()):e.active===!1?this._activate(0):this.active.length&&!t.contains(this.element[0],this.active[0])?this.headers.length===this.headers.find(".ui-state-disabled").length?(e.active=!1,this.active=t()):this._activate(Math.max(0,e.active-1)):e.active=this.headers.index(this.active),this._destroyIcons(),this._refresh()},_processPanels:function(){var t=this.headers,e=this.panels;this.headers=this.element.find(this.options.header),this._addClass(this.headers,"ui-accordion-header ui-accordion-header-collapsed","ui-state-default"),this.panels=this.headers.next().filter(":not(.ui-accordion-content-active)").hide(),this._addClass(this.panels,"ui-accordion-content","ui-helper-reset ui-widget-content"),e&&(this._off(t.not(this.headers)),this._off(e.not(this.panels)))},_refresh:function(){var e,i=this.options,s=i.heightStyle,n=this.element.parent();this.active=this._findActive(i.active),this._addClass(this.active,"ui-accordion-header-active","ui-state-active")._removeClass(this.active,"ui-accordion-header-collapsed"),this._addClass(this.active.next(),"ui-accordion-content-active"),this.active.next().show(),this.headers.attr("role","tab").each(function(){var e=t(this),i=e.uniqueId().attr("id"),s=e.next(),n=s.uniqueId().attr("id");e.attr("aria-controls",n),s.attr("aria-labelledby",i)}).next().attr("role","tabpanel"),this.headers.not(this.active).attr({"aria-selected":"false","aria-expanded":"false",tabIndex:-1}).next().attr({"aria-hidden":"true"}).hide(),this.active.length?this.active.attr({"aria-selected":"true","aria-expanded":"true",tabIndex:0}).next().attr({"aria-hidden":"false"}):this.headers.eq(0).attr("tabIndex",0),this._createIcons(),this._setupEvents(i.event),"fill"===s?(e=n.height(),this.element.siblings(":visible").each(function(){var i=t(this),s=i.css("position");"absolute"!==s&&"fixed"!==s&&(e-=i.outerHeight(!0))}),this.headers.each(function(){e-=t(this).outerHeight(!0)}),this.headers.next().each(function(){t(this).height(Math.max(0,e-t(this).innerHeight()+t(this).height()))}).css("overflow","auto")):"auto"===s&&(e=0,this.headers.next().each(function(){var i=t(this).is(":visible");i||t(this).show(),e=Math.max(e,t(this).css("height","").height()),i||t(this).hide()}).height(e))},_activate:function(e){var i=this._findActive(e)[0];i!==this.active[0]&&(i=i||this.active[0],this._eventHandler({target:i,currentTarget:i,preventDefault:t.noop}))},_findActive:function(e){return"number"==typeof e?this.headers.eq(e):t()},_setupEvents:function(e){var i={keydown:"_keydown"};e&&t.each(e.split(" "),function(t,e){i[e]="_eventHandler"}),this._off(this.headers.add(this.headers.next())),this._on(this.headers,i),this._on(this.headers.next(),{keydown:"_panelKeyDown"}),this._hoverable(this.headers),this._focusable(this.headers)},_eventHandler:function(e){var i,s,n=this.options,o=this.active,a=t(e.currentTarget),r=a[0]===o[0],l=r&&n.collapsible,h=l?t():a.next(),c=o.next(),u={oldHeader:o,oldPanel:c,newHeader:l?t():a,newPanel:h};e.preventDefault(),r&&!n.collapsible||this._trigger("beforeActivate",e,u)===!1||(n.active=l?!1:this.headers.index(a),this.active=r?t():a,this._toggle(u),this._removeClass(o,"ui-accordion-header-active","ui-state-active"),n.icons&&(i=o.children(".ui-accordion-header-icon"),this._removeClass(i,null,n.icons.activeHeader)._addClass(i,null,n.icons.header)),r||(this._removeClass(a,"ui-accordion-header-collapsed")._addClass(a,"ui-accordion-header-active","ui-state-active"),n.icons&&(s=a.children(".ui-accordion-header-icon"),this._removeClass(s,null,n.icons.header)._addClass(s,null,n.icons.activeHeader)),this._addClass(a.next(),"ui-accordion-content-active")))},_toggle:function(e){var i=e.newPanel,s=this.prevShow.length?this.prevShow:e.oldPanel;this.prevShow.add(this.prevHide).stop(!0,!0),this.prevShow=i,this.prevHide=s,this.options.animate?this._animate(i,s,e):(s.hide(),i.show(),this._toggleComplete(e)),s.attr({"aria-hidden":"true"}),s.prev().attr({"aria-selected":"false","aria-expanded":"false"}),i.length&&s.length?s.prev().attr({tabIndex:-1,"aria-expanded":"false"}):i.length&&this.headers.filter(function(){return 0===parseInt(t(this).attr("tabIndex"),10)}).attr("tabIndex",-1),i.attr("aria-hidden","false").prev().attr({"aria-selected":"true","aria-expanded":"true",tabIndex:0})},_animate:function(t,e,i){var s,n,o,a=this,r=0,l=t.css("box-sizing"),h=t.length&&(!e.length||t.index()<e.index()),c=this.options.animate||{},u=h&&c.down||c,d=function(){a._toggleComplete(i)};return"number"==typeof u&&(o=u),"string"==typeof u&&(n=u),n=n||u.easing||c.easing,o=o||u.duration||c.duration,e.length?t.length?(s=t.show().outerHeight(),e.animate(this.hideProps,{duration:o,easing:n,step:function(t,e){e.now=Math.round(t)}}),t.hide().animate(this.showProps,{duration:o,easing:n,complete:d,step:function(t,i){i.now=Math.round(t),"height"!==i.prop?"content-box"===l&&(r+=i.now):"content"!==a.options.heightStyle&&(i.now=Math.round(s-e.outerHeight()-r),r=0)}}),void 0):e.animate(this.hideProps,o,n,d):t.animate(this.showProps,o,n,d)},_toggleComplete:function(t){var e=t.oldPanel,i=e.prev();this._removeClass(e,"ui-accordion-content-active"),this._removeClass(i,"ui-accordion-header-active")._addClass(i,"ui-accordion-header-collapsed"),e.length&&(e.parent()[0].className=e.parent()[0].className),this._trigger("activate",null,t)}}),t.ui.escapeSelector=function(){var t=/([!"#$%&'()*+,./:;<=>?@[\]^`{|}~])/g;return function(e){return e.replace(t,"\\$1")}}(),t.ui.safeActiveElement=function(t){var e;try{e=t.activeElement}catch(i){e=t.body}return e||(e=t.body),e.nodeName||(e=t.body),e},t.widget("ui.tabs",{version:"1.12.1",delay:300,options:{active:null,classes:{"ui-tabs":"ui-corner-all","ui-tabs-nav":"ui-corner-all","ui-tabs-panel":"ui-corner-bottom","ui-tabs-tab":"ui-corner-top"},collapsible:!1,event:"click",heightStyle:"content",hide:null,show:null,activate:null,beforeActivate:null,beforeLoad:null,load:null},_isLocal:function(){var t=/#.*$/;return function(e){var i,s;i=e.href.replace(t,""),s=location.href.replace(t,"");try{i=decodeURIComponent(i)}catch(n){}try{s=decodeURIComponent(s)}catch(n){}return e.hash.length>1&&i===s}}(),_create:function(){var e=this,i=this.options;this.running=!1,this._addClass("ui-tabs","ui-widget ui-widget-content"),this._toggleClass("ui-tabs-collapsible",null,i.collapsible),this._processTabs(),i.active=this._initialActive(),t.isArray(i.disabled)&&(i.disabled=t.unique(i.disabled.concat(t.map(this.tabs.filter(".ui-state-disabled"),function(t){return e.tabs.index(t)}))).sort()),this.active=this.options.active!==!1&&this.anchors.length?this._findActive(i.active):t(),this._refresh(),this.active.length&&this.load(i.active)},_initialActive:function(){var e=this.options.active,i=this.options.collapsible,s=location.hash.substring(1);return null===e&&(s&&this.tabs.each(function(i,n){return t(n).attr("aria-controls")===s?(e=i,!1):void 0}),null===e&&(e=this.tabs.index(this.tabs.filter(".ui-tabs-active"))),(null===e||-1===e)&&(e=this.tabs.length?0:!1)),e!==!1&&(e=this.tabs.index(this.tabs.eq(e)),-1===e&&(e=i?!1:0)),!i&&e===!1&&this.anchors.length&&(e=0),e},_getCreateEventData:function(){return{tab:this.active,panel:this.active.length?this._getPanelForTab(this.active):t()}},_tabKeydown:function(e){var i=t(t.ui.safeActiveElement(this.document[0])).closest("li"),s=this.tabs.index(i),n=!0;if(!this._handlePageNav(e)){switch(e.keyCode){case t.ui.keyCode.RIGHT:case t.ui.keyCode.DOWN:s++;break;case t.ui.keyCode.UP:case t.ui.keyCode.LEFT:n=!1,s--;break;case t.ui.keyCode.END:s=this.anchors.length-1;break;case t.ui.keyCode.HOME:s=0;break;case t.ui.keyCode.SPACE:return e.preventDefault(),clearTimeout(this.activating),this._activate(s),void 0;case t.ui.keyCode.ENTER:return e.preventDefault(),clearTimeout(this.activating),this._activate(s===this.options.active?!1:s),void 0;default:return}e.preventDefault(),clearTimeout(this.activating),s=this._focusNextTab(s,n),e.ctrlKey||e.metaKey||(i.attr("aria-selected","false"),this.tabs.eq(s).attr("aria-selected","true"),this.activating=this._delay(function(){this.option("active",s)},this.delay))}},_panelKeydown:function(e){this._handlePageNav(e)||e.ctrlKey&&e.keyCode===t.ui.keyCode.UP&&(e.preventDefault(),this.active.trigger("focus"))},_handlePageNav:function(e){return e.altKey&&e.keyCode===t.ui.keyCode.PAGE_UP?(this._activate(this._focusNextTab(this.options.active-1,!1)),!0):e.altKey&&e.keyCode===t.ui.keyCode.PAGE_DOWN?(this._activate(this._focusNextTab(this.options.active+1,!0)),!0):void 0},_findNextTab:function(e,i){function s(){return e>n&&(e=0),0>e&&(e=n),e}for(var n=this.tabs.length-1;-1!==t.inArray(s(),this.options.disabled);)e=i?e+1:e-1;return e},_focusNextTab:function(t,e){return t=this._findNextTab(t,e),this.tabs.eq(t).trigger("focus"),t},_setOption:function(t,e){return"active"===t?(this._activate(e),void 0):(this._super(t,e),"collapsible"===t&&(this._toggleClass("ui-tabs-collapsible",null,e),e||this.options.active!==!1||this._activate(0)),"event"===t&&this._setupEvents(e),"heightStyle"===t&&this._setupHeightStyle(e),void 0)},_sanitizeSelector:function(t){return t?t.replace(/[!"$%&'()*+,.\/:;<=>?@\[\]\^`{|}~]/g,"\\$&"):""},refresh:function(){var e=this.options,i=this.tablist.children(":has(a[href])");e.disabled=t.map(i.filter(".ui-state-disabled"),function(t){return i.index(t)}),this._processTabs(),e.active!==!1&&this.anchors.length?this.active.length&&!t.contains(this.tablist[0],this.active[0])?this.tabs.length===e.disabled.length?(e.active=!1,this.active=t()):this._activate(this._findNextTab(Math.max(0,e.active-1),!1)):e.active=this.tabs.index(this.active):(e.active=!1,this.active=t()),this._refresh()},_refresh:function(){this._setOptionDisabled(this.options.disabled),this._setupEvents(this.options.event),this._setupHeightStyle(this.options.heightStyle),this.tabs.not(this.active).attr({"aria-selected":"false","aria-expanded":"false",tabIndex:-1}),this.panels.not(this._getPanelForTab(this.active)).hide().attr({"aria-hidden":"true"}),this.active.length?(this.active.attr({"aria-selected":"true","aria-expanded":"true",tabIndex:0}),this._addClass(this.active,"ui-tabs-active","ui-state-active"),this._getPanelForTab(this.active).show().attr({"aria-hidden":"false"})):this.tabs.eq(0).attr("tabIndex",0)},_processTabs:function(){var e=this,i=this.tabs,s=this.anchors,n=this.panels;this.tablist=this._getList().attr("role","tablist"),this._addClass(this.tablist,"ui-tabs-nav","ui-helper-reset ui-helper-clearfix ui-widget-header"),this.tablist.on("mousedown"+this.eventNamespace,"> li",function(e){t(this).is(".ui-state-disabled")&&e.preventDefault()}).on("focus"+this.eventNamespace,".ui-tabs-anchor",function(){t(this).closest("li").is(".ui-state-disabled")&&this.blur()}),this.tabs=this.tablist.find("> li:has(a[href])").attr({role:"tab",tabIndex:-1}),this._addClass(this.tabs,"ui-tabs-tab","ui-state-default"),this.anchors=this.tabs.map(function(){return t("a",this)[0]}).attr({role:"presentation",tabIndex:-1}),this._addClass(this.anchors,"ui-tabs-anchor"),this.panels=t(),this.anchors.each(function(i,s){var n,o,a,r=t(s).uniqueId().attr("id"),l=t(s).closest("li"),h=l.attr("aria-controls");e._isLocal(s)?(n=s.hash,a=n.substring(1),o=e.element.find(e._sanitizeSelector(n))):(a=l.attr("aria-controls")||t({}).uniqueId()[0].id,n="#"+a,o=e.element.find(n),o.length||(o=e._createPanel(a),o.insertAfter(e.panels[i-1]||e.tablist)),o.attr("aria-live","polite")),o.length&&(e.panels=e.panels.add(o)),h&&l.data("ui-tabs-aria-controls",h),l.attr({"aria-controls":a,"aria-labelledby":r}),o.attr("aria-labelledby",r)}),this.panels.attr("role","tabpanel"),this._addClass(this.panels,"ui-tabs-panel","ui-widget-content"),i&&(this._off(i.not(this.tabs)),this._off(s.not(this.anchors)),this._off(n.not(this.panels)))},_getList:function(){return this.tablist||this.element.find("ol, ul").eq(0)},_createPanel:function(e){return t("<div>").attr("id",e).data("ui-tabs-destroy",!0)},_setOptionDisabled:function(e){var i,s,n;for(t.isArray(e)&&(e.length?e.length===this.anchors.length&&(e=!0):e=!1),n=0;s=this.tabs[n];n++)i=t(s),e===!0||-1!==t.inArray(n,e)?(i.attr("aria-disabled","true"),this._addClass(i,null,"ui-state-disabled")):(i.removeAttr("aria-disabled"),this._removeClass(i,null,"ui-state-disabled"));this.options.disabled=e,this._toggleClass(this.widget(),this.widgetFullName+"-disabled",null,e===!0)},_setupEvents:function(e){var i={};e&&t.each(e.split(" "),function(t,e){i[e]="_eventHandler"}),this._off(this.anchors.add(this.tabs).add(this.panels)),this._on(!0,this.anchors,{click:function(t){t.preventDefault()}}),this._on(this.anchors,i),this._on(this.tabs,{keydown:"_tabKeydown"}),this._on(this.panels,{keydown:"_panelKeydown"}),this._focusable(this.tabs),this._hoverable(this.tabs)},_setupHeightStyle:function(e){var i,s=this.element.parent();"fill"===e?(i=s.height(),i-=this.element.outerHeight()-this.element.height(),this.element.siblings(":visible").each(function(){var e=t(this),s=e.css("position");"absolute"!==s&&"fixed"!==s&&(i-=e.outerHeight(!0))}),this.element.children().not(this.panels).each(function(){i-=t(this).outerHeight(!0)}),this.panels.each(function(){t(this).height(Math.max(0,i-t(this).innerHeight()+t(this).height()))}).css("overflow","auto")):"auto"===e&&(i=0,this.panels.each(function(){i=Math.max(i,t(this).height("").height())}).height(i))},_eventHandler:function(e){var i=this.options,s=this.active,n=t(e.currentTarget),o=n.closest("li"),a=o[0]===s[0],r=a&&i.collapsible,l=r?t():this._getPanelForTab(o),h=s.length?this._getPanelForTab(s):t(),c={oldTab:s,oldPanel:h,newTab:r?t():o,newPanel:l};e.preventDefault(),o.hasClass("ui-state-disabled")||o.hasClass("ui-tabs-loading")||this.running||a&&!i.collapsible||this._trigger("beforeActivate",e,c)===!1||(i.active=r?!1:this.tabs.index(o),this.active=a?t():o,this.xhr&&this.xhr.abort(),h.length||l.length||t.error("jQuery UI Tabs: Mismatching fragment identifier."),l.length&&this.load(this.tabs.index(o),e),this._toggle(e,c))},_toggle:function(e,i){function s(){o.running=!1,o._trigger("activate",e,i)}function n(){o._addClass(i.newTab.closest("li"),"ui-tabs-active","ui-state-active"),a.length&&o.options.show?o._show(a,o.options.show,s):(a.show(),s())}var o=this,a=i.newPanel,r=i.oldPanel;this.running=!0,r.length&&this.options.hide?this._hide(r,this.options.hide,function(){o._removeClass(i.oldTab.closest("li"),"ui-tabs-active","ui-state-active"),n()}):(this._removeClass(i.oldTab.closest("li"),"ui-tabs-active","ui-state-active"),r.hide(),n()),r.attr("aria-hidden","true"),i.oldTab.attr({"aria-selected":"false","aria-expanded":"false"}),a.length&&r.length?i.oldTab.attr("tabIndex",-1):a.length&&this.tabs.filter(function(){return 0===t(this).attr("tabIndex")}).attr("tabIndex",-1),a.attr("aria-hidden","false"),i.newTab.attr({"aria-selected":"true","aria-expanded":"true",tabIndex:0})},_activate:function(e){var i,s=this._findActive(e);s[0]!==this.active[0]&&(s.length||(s=this.active),i=s.find(".ui-tabs-anchor")[0],this._eventHandler({target:i,currentTarget:i,preventDefault:t.noop}))},_findActive:function(e){return e===!1?t():this.tabs.eq(e)},_getIndex:function(e){return"string"==typeof e&&(e=this.anchors.index(this.anchors.filter("[href$='"+t.ui.escapeSelector(e)+"']"))),e},_destroy:function(){this.xhr&&this.xhr.abort(),this.tablist.removeAttr("role").off(this.eventNamespace),this.anchors.removeAttr("role tabIndex").removeUniqueId(),this.tabs.add(this.panels).each(function(){t.data(this,"ui-tabs-destroy")?t(this).remove():t(this).removeAttr("role tabIndex aria-live aria-busy aria-selected aria-labelledby aria-hidden aria-expanded")}),this.tabs.each(function(){var e=t(this),i=e.data("ui-tabs-aria-controls");i?e.attr("aria-controls",i).removeData("ui-tabs-aria-controls"):e.removeAttr("aria-controls")}),this.panels.show(),"content"!==this.options.heightStyle&&this.panels.css("height","")},enable:function(e){var i=this.options.disabled;i!==!1&&(void 0===e?i=!1:(e=this._getIndex(e),i=t.isArray(i)?t.map(i,function(t){return t!==e?t:null}):t.map(this.tabs,function(t,i){return i!==e?i:null})),this._setOptionDisabled(i))},disable:function(e){var i=this.options.disabled;if(i!==!0){if(void 0===e)i=!0;else{if(e=this._getIndex(e),-1!==t.inArray(e,i))return;i=t.isArray(i)?t.merge([e],i).sort():[e]}this._setOptionDisabled(i)}},load:function(e,i){e=this._getIndex(e);var s=this,n=this.tabs.eq(e),o=n.find(".ui-tabs-anchor"),a=this._getPanelForTab(n),r={tab:n,panel:a},l=function(t,e){"abort"===e&&s.panels.stop(!1,!0),s._removeClass(n,"ui-tabs-loading"),a.removeAttr("aria-busy"),t===s.xhr&&delete s.xhr};this._isLocal(o[0])||(this.xhr=t.ajax(this._ajaxSettings(o,i,r)),this.xhr&&"canceled"!==this.xhr.statusText&&(this._addClass(n,"ui-tabs-loading"),a.attr("aria-busy","true"),this.xhr.done(function(t,e,n){setTimeout(function(){a.html(t),s._trigger("load",i,r),l(n,e)},1)}).fail(function(t,e){setTimeout(function(){l(t,e)},1)})))},_ajaxSettings:function(e,i,s){var n=this;return{url:e.attr("href").replace(/#.*$/,""),beforeSend:function(e,o){return n._trigger("beforeLoad",i,t.extend({jqXHR:e,ajaxSettings:o},s))}}},_getPanelForTab:function(e){var i=t(e).attr("aria-controls");return this.element.find(this._sanitizeSelector("#"+i))}}),t.uiBackCompat!==!1&&t.widget("ui.tabs",t.ui.tabs,{_processTabs:function(){this._superApply(arguments),this._addClass(this.tabs,"ui-tab")}}),t.ui.tabs});
	}

	// BEGIN ACTUAL CODE THAT ENHANCES USER CONTENT
	!function(){

	  function makeJqueryUIWidgetsFromUserContent () {
	    var $widgets = $('.user_content:not(.jqueryUIWidgetized)').addClass('jqueryUIWidgetized')
	      .find('.erau-accordion:not(.jqueryUIWidgetized),.erau-ui-tabs:not(.jqueryUIWidgetized)')

	    if ($widgets.length) {
	      $widgets.addClass('jqueryUIWidgetized').show()

		  // all of the `.is('.ui-*` and `:not(.ui-*` checks are to skip doing anything if canvas has already widgetized things
		  // 062519 the accordion and tabs fuctions suddenly disappeared so we had to put these protections in place
		  if (typeof $.fn.pluginname !== 'undefined') {
		  	$widgets.filter('.erau-accordion:not(.ui-accordion)').accordion()
		  }
		  if (typeof $.fn.pluginname !== 'undefined') {
		  	$widgets.filter('.erau-ui-tabs:not(.ui-tabs)').each(function () { $(this).tabs() })
		  }
	    }
	  }

	  try {
	    $.subscribe('userContent/change', makeJqueryUIWidgetsFromUserContent)
	  } catch (e) { }

	  $(document).bind('user_content_change', makeJqueryUIWidgetsFromUserContent)
	  setInterval(makeJqueryUIWidgetsFromUserContent, 5000)
	  makeJqueryUIWidgetsFromUserContent()
	}();	

	// Enable fullscreen for iframe videos
	onElementRendered('iframe', function(el) {
		$('iframe').attr("webkitallowfullscreen", "true").attr("allowFullScreen", "true").attr("mozallowfullscreen", "true");
	}, 4);
	onElementRendered('#wiki_page_show div.show-content', function(el) {
		$('iframe').attr("webkitallowfullscreen", "true").attr("allowFullScreen", "true").attr("mozallowfullscreen", "true");
	}, 4);
	onElementRendered('div.user_content', function(el) {
		$('iframe').attr("webkitallowfullscreen", "true").attr("allowFullScreen", "true").attr("mozallowfullscreen", "true");
	});

	// Add warning message next to grade sync button
	// IT Support Ticket 41056
	if (window.location.href.indexOf('/settings') !== -1) {
		var SISwarning = $('<div/>', {
			'class': 'alert alert-warning',
			'role': 'alert',
			'style': 'margin-bottom: 12px;',
			'html': '<ol><li><strong>Do not try to sync grades multiple times.</strong> The&nbsp;grade syncing button should only be used&nbsp;once. If&nbsp;there are incomplete grades in Campus Solutions after using the grade sync function, you will need to adjust grades manually in Campus&nbsp;Solutions.</li><li><strong>Large classes may take longer to&nbsp;sync.</strong> Error&nbsp;messages, such as <code>Error: Net:: ReadTimeout.</code>, indicate Campus Solutions is under heavy&nbsp;load. The&nbsp;message will update to <em>Synced</em> when the operation is&nbsp;complete.</li></ol>',
		});
		SISwarning.insertBefore('#publish_grades_link');
	}

});