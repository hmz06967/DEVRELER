function get_yt_box(j,o){
	

	var hm ='',
		dl = j[0] !=null ? (typeof j[0][0]  !=="undefined" ? j[0][0] : false) : false;
		
	hm += '<div class="video_nav_main" id="indir_get_info_plugin">	<div class="_miff" style=" text-align: -webkit-left; text-align: left; margin:0"><div class="_mbig"> <h4 class="_mig_dn"  >'+j[1]+'</h4></div><!--Video-indir-linkler--><div class="video_control-bar"><div class="indi-nav" style="background: #fff;"> <div class="_mcigx" style="  width: 100%;  height: 100%;"> <div class="mhof_0"><div class="mginf_" id="img_play" style=" background: url('+j[3]+');height: 100px;max-width: 450px;background-size: cover;  background-position: 50% 50%; width: auto;padding-left: 0;"><div class="get_videos-embed"></div></div></div>';
						
					
	if(false){
		//error durumunda
	}else{
		
	
		hm += '<div class="nv_alt-pn" style="color: #606060;font-size: 12px; "><div class="_mgig"><div class="_mhig0" style=" padding-top: 0; ">  <div class="sc-button-group sc-button-group-small" > <div  onclick="$.listen.ply(this);" class="too sc-button-play sc-button  sc-button-responsive" title="Klibi dinle." style="color: #DF1A1A;"><i class="fa fa-play "></i><span class="fa-pauses"><i class="fa fa-pause"></i> </span></div><div  class="fa fa-share-alt sc-button-share sc-button  sc-button-responsive"   aria-haspopup="true" data-action="$.shr.show" title="Klibi paylaş." ></div><a href="#like" data-action="$.User.addFavorite" value="<?=getYoutubeID($urls)?>" class="sc-button  sc-button-responsive convertion_digges fa fa-heart-o fa-heart"><span class="yt-uix-button-content" r-number="-1"></span></a><div   for-class="_shyii" class="fa fa-ellipsis-h sc-button  sc-button-responsive sc-button-addtoset" title="Diğer seçenekler"></div></div><div class="now-denbutn" >';
		
		hm += (typeof dl["text"] == "string") ? dl["text"] :  dl;
		
		var u = dl!=false && typeof dl== 'string' ? dl.split('href="').pop().split('"').shift() : "#";
		
		hm +='<a href="'+u+'"  id="get_indir"  title="Dosyayı İndir" id="jural_links" class="jac-.ndo" ><span><i class="icon-arrow-left"></i></span> Şimdi İndir</a></div></div></div></div><div class="pl_oynat"><div class="pl_hkd"><span><i class="fa fa-play"></i></span></div></div></div>';
		
		if(typeof j[1] == "string"){
			
			hm += '<div class="diger_download"><div class="other_nbh" ><span class="diger_vvv"> Başka formatlarda indirin.:</span></div>';
	
			if(typeof dl == "object"){
				
				hm += '<table class="w3-table-all"><thead><tr><th>Uzantı</th><th>Çözünürlük</th><th>Boyut</th><th>İndir</th></tr></thead><tbody>';

				for(i=j[0].length-1;i>=0;i--){
					
					var $km = j[0][i],
						fn = j[1].replaceAlls()+$km['ext'];
						
					hm += '<tr class="menu-links" id="get_indir" ><td >'+$km["ext"]+'</td><td>'+$km["quality"]+'</td><td>'+$km["size"]+'</td><td ><a href="'+$km["url"]+'" class="cw_dn dial_muhaner" id="get-comlo_token" target="_blank" download="'+fn+'""><b><span class="vc__"> İndir</span></b></a></td></tr>';
			
				}	 
			
				hm +="</tbody></table>";
				
			}else{
				for(i=j[0].length-1;i>=0;i--){
					hm+=j[0][i];
				}
			}
			
			if(typeof j[6] == "object" && isYotube(j[6]))
				hm+='<div class="_btyi mp_conIndo" style="background: #fff;"><div class="menu-links"><a title="Format: mp3 Olarak İndir" href="/indir/'+getVideFromId(j[6])+'" data-ga-event="send;event;result;click;101" data-action="DownAnlyz" class="cw_dn dial_muhaner" id="get-comlo_token" style="display: flow-root;border-bottom: 1px solid #ddd;color: #001853;"><div class="_gnstart_text"></div> <span style="float: left;"><i class="fa fa-arrow-down" style="margin-right:10px"></i>  Mp3 Ses 192k <i class="fas fa-volume-up fa-xs" style="    color: #b3aeae;"></i></span><span class="vc__" style="float: right;"> İndir</span></a></div></div>';
			
		/*	hm += '<div class="_btyi mp_conIndo" style="display:none"><form action="/indir" method="POST"> <input class="mp_con_id" type="hidden" value="<?=$urls?>" name="hly_key"> <input type="hidden" value="<?=$title?>,<?=$duration?>" name="tit_iss"><div class="bty_convert_button"><input type="hidden" name="meta_indir"><button type="submit" class="x_actionskey" name="meta_key"><a class="_btfa" href="#dosya_indir"> <span class="_btoy _miff"><i class="fa fa-file-audio-o"></i>Mp3 Müzik 128k <span class="vc__"> İndir</span></span></a></button></div> </form></div></div>';	*/
			
		}	

		hm += '<div class="err_get-video" id="promo"></div></div></div></div> </div>';


	
		var d = {
			url:o.a,
			sites:false,
			title:j[1]
		}

		GetVideoDownload(d);
	}
	
	return hm;
}