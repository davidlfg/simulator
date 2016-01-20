(function($) {
  Drupal.behaviors.fieldMonto = {
    attach : function(context, settings) {
      //Codigo para formatiar monedas
      $(".field-simulator-crp").keyup(function() {
        var entrada = $(this).val();
        entrada = (entrada + '').replace(/[^0-9+\.]/g, '');
        entrada = replaceAll(entrada, ".", "" );
        if(!isNaN(entrada)) {
          $(this).val(numberFormat(entrada));
        }
      });
    }
  }
  // Function replace
  function replaceAll(text, busca, reemplaza) {
    while (text.toString().indexOf(busca) != -1)
      text = text.toString().replace(busca,reemplaza);
    return text;
  }
  // function format number
  function numberFormat(entrada) {
    var formatNumber = {
      separador: ",", // decimales
      sepDecimal: '.', // 
      formatear:function (num) {
        num +='';
        var splitStr = num.split('.');
        var splitLeft = splitStr[0];
        var splitRight = splitStr.length > 1 ? this.sepDecimal + splitStr[1] : '';
        var regx = /(\d+)(\d{3})/;
        while (regx.test(splitLeft)) {
        splitLeft = splitLeft.replace(regx, '$1' + this.separador + '$2');
        }
        return this.simbol + splitLeft  +splitRight;
      },
      newFuuu: function(num, simbol){
        this.simbol = simbol || '';
        return this.formatear(num);
      }
    }
    return formatNumber.newFuuu(entrada);
  }
})(jQuery);